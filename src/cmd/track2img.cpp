#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_TRACK2IMAGE {

    std::string             inp_fname;
    std::string             feature = "DEC_segmentLength";
    std::string             weights;
    std::string             template_image = "";
    float                   template_upsampleFactor = 1.0;
    std::tuple<float,int>   smoothing(0.0f, 0);
    std::vector<float>      bbox;
    std::vector<float>      voxDim;
    int                     sfRes = 0;
    float                   sfSmoothing = 0;
    int                     shOrder = 0;

    std::string             mask_image;
    CLI::Option*            mask_option;
    int                     mask_label;
    CLI::Option*            mask_label_option;
    std::string             out_fname;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;

}

using namespace CMDARGS_TRACK2IMAGE;

void run_track2img()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;

    if(!ensureVTKorTCK(inp_fname)) return;
    
    if ((feature!="streamlineCount") && (feature!="segmentLength") && (feature!="DEC_segmentLength") ) {
        std::cout << "Feature can be \"streamlineCount\", \"segmentLength\" or \"DEC_segmentLength\" " << std::endl << std::flush;
        return;
    }

    if ((template_image=="") && (bbox.empty()) && (voxDim.empty()) ) {
        voxDim.push_back(1.0f);
    }

    if ((template_image!="") && ((bbox.empty()==false) || (voxDim.empty()==false))) {
        std::cout << "Only one of --template, --bbox or --voxDim must be defined." << std::endl << std::flush;
        return;
    }
    
    if ((bbox.empty()==false) && ((template_image!="")  || (voxDim.empty()==false))) {
        std::cout << "Only one of --template, --bbox or --voxDim must be defined." << std::endl << std::flush;
        return;
    } else if (bbox.empty()==false) {
        if ( (bbox.size()!=6) && (bbox.size()!=7) && (bbox.size()!=9) ) {
            std::cout << "Options for bbox are: \n    (i) \"x_min x_max y_min y_max z_min z_max\" (an isotropic voxel dimension of 1 will be assumed.)\n   (ii) \"x_min x_max y_min y_max z_min z_max voxDim\" (an isotropic voxel dimension of voxDim will be assumed.)\n  (iii) \"x_min x_max y_min y_max z_min z_max xVoxDim yVoxDim zVoxDim\" " << std::endl << std::flush;
            return;
        }
    }

    if ( (voxDim.empty()==false) && ((template_image!="")  ||   (bbox.empty()==false)) ) {
        std::cout << "Only one of --template, --bbox or --voxDim must be defined." << std::endl << std::flush;
        return;
    } else if (voxDim.empty()==false) {
        if ( (voxDim.size()!=1) && (voxDim.size()!=3) ) {
            std::cout << "Options for voxDim are: \n    (i) a single value that will be used for all dimensions\n   (ii) one value for each three dimensions seperated by space" << std::endl << std::flush;
            return;
        }
    }
    
    if (std::get<0>(smoothing)<0) {
        std::cout << "Standard deviation for anisotropic Gaussian smoothing can't be smaller than 0." << std::endl << std::flush;
        return;
    }
    if (std::get<1>(smoothing)<0) {
        std::cout << "Computation density for anisotropic Gaussian smoothing can't be smaller than 0." << std::endl << std::flush;
        return;
    }

    // Initialize tractogram
    NIBR::TractogramReader tractogram(inp_fname);

    if (tractogram.numberOfStreamlines<1) {
        std::cout << "Empty tractogram" << std::endl;
        //return;
    }

    // Prep for spherical function output
    if (sfRes>0) {
        if ( (sfRes%2 == 0) || (sfRes>31) ){
            std::cout << "Spherical domain resolution can only be an odd positive integer up to 31."  << std::endl << std::flush;
            return;
        }
        NIBR::SF::init(true,sfRes);
    }

    // Prep for spherical harmonics output
    if (shOrder>0) {
        if (shOrder%2 == 1) {
            std::cout << "Spherical harmonics order should be a positive even integer."  << std::endl << std::flush;
            return;
        }
    }


    // Do the mapping
    if (feature=="segmentLength") {

        NIBR::Image<float> img;

        // Use template image
        if (template_image!="") {
            NIBR::Image<float> tempImg(template_image);
            tempImg.readHeader();
            if (sfRes>0) {
                int64_t imgDims[4] = {tempImg.imgDims[0], tempImg.imgDims[1], tempImg.imgDims[2], int64_t(SF::getSFCoords().size())};
                img.create(4, &imgDims[0], tempImg.pixDims, tempImg.ijk2xyz, false);
            }
            else
                img.createFromTemplate(tempImg,template_upsampleFactor,false);
        }

        // Use bounding box
        if (bbox.empty()==false) {
            if (sfRes>0) {
                std::vector<float> bb(bbox.begin(),  bbox.begin()+6);
                std::vector<float> pd(bbox.begin()+6,bbox.end());
                bb.push_back(-0.5);
                bb.push_back(int64_t(SF::getSFCoords().size())-0.5);
                if (pd.size()==1) {
                    pd.push_back(pd[0]);
                    pd.push_back(pd[0]);
                } else if (pd.size()!=3) {
                    std::cout << "pixel dimension should be either a single isotropic value or 3 values corresponding to x, y and z dimensions" << std::endl << std::flush;
                    return;
                }
                pd.push_back(1.0f);
                img.createFromBoundingBox(4,bb,pd,false);
            } else {
                std::vector<float> bb(bbox.begin(),  bbox.begin()+6);
                std::vector<float> pd(bbox.begin()+6,bbox.end());
                img.createFromBoundingBox(3,bb,pd,false);
            }
        }

        // Use voxDim
        if (voxDim.empty()==false) {
            std::vector<float> bb = getTractogramBBox(&tractogram);
            if (sfRes>0) {
                bb.push_back(-0.5);
                bb.push_back(int64_t(SF::getSFCoords().size())-0.5);
                img.createFromBoundingBox(4,bb,voxDim,false);
            } else {
                img.createFromBoundingBox(3,bb,voxDim,false);
            }
        }

    
        Tractogram2ImageMapper<float> gridder(&tractogram,&img);

        if (*mask_option) {
            NIBR::Image<int> mask(mask_image);
            if (*mask_label_option) {
                if (!gridder.setMask(&mask,mask_label)) return;
            } else {
                if (!gridder.setMask(&mask))            return;
            }            
        }

        gridder.anisotropicSmoothing(smoothing);

        if (sfRes>0) {
            allocateGrid_4segmentLength_sf(&gridder);
            gridder.run(processor_4segmentLength_sf<float>, outputCompiler_4segmentLength_sf<float>);
            deallocateGrid_4segmentLength_sf(&gridder);
        }
        else if (weights!="") {
            gridder.setWeights(weights, STREAMLINE_WEIGHT);
            allocateGrid_4segmentLength(&gridder);
            gridder.run(processor_4segmentLength_weighted<float>,    outputCompiler_4segmentLength<float>);
            deallocateGrid_4segmentLength(&gridder);
        } else {
            allocateGrid_4segmentLength(&gridder);
            gridder.run(processor_4segmentLength<float>,             outputCompiler_4segmentLength<float>);
            deallocateGrid_4segmentLength(&gridder);
        }
        

        if (shOrder>0) {
            NIBR::Image<float> out;
            sf2sh(&out,&img,SF::getSFCoords(),shOrder);
            out.write(removeFileExtension(out_fname) + "_SH.nii.gz");
        }

        img.write(out_fname);

        if (sfSmoothing>0) {
            SF_Image sfImg(out_fname,true);
            sfImg.read();
            sfImg.setOutsideVal(0.0f);
            sfImg.smooth(sfSmoothing);
            sfImg.write(removeFileExtension(out_fname) + "_smooth.nii.gz");

            if (shOrder>0) {
                NIBR::Image<float> smShOut;
                sf2sh(&smShOut,&sfImg,SF::getSFCoords(),shOrder);
                smShOut.write(removeFileExtension(out_fname) + "_smooth_SH.nii.gz");
            }
        }
        
    }

    // Do the mapping
    if (feature=="DEC_segmentLength") {

        if (sfRes>0) {
            std::cout << "Spherical harmonics output can't be written for DEC_segmentLength feature."  << std::endl << std::flush;
            return;
        }

        NIBR::Image<float> img;

        // Use template image
        if (template_image!="") {
            NIBR::Image<float> tempImg(template_image);
            tempImg.readHeader();

            img.createFromTemplate(tempImg,template_upsampleFactor,false);
            img.numberOfDimensions = 4;
            img.imgDims[3] = 3;
            img.pixDims[3] = 1;

        }

        // Use bounding box
        if (bbox.empty()==false) {
            std::vector<float> bb(bbox.begin(),  bbox.begin()+6);
            std::vector<float> pd(bbox.begin()+6,bbox.end());

            bb.push_back(-0.5);         // This is the 4th, i.e., the DEC dimension
            bb.push_back( 2.5);

            if (pd.size()==1) {
                pd.push_back(pd[0]);
                pd.push_back(pd[0]);
            }
            pd.push_back(1);            // Set DEC pixel dimension to 1

            img.createFromBoundingBox(4, bb, pd, false);
        }

        // Use voxDim
        if (voxDim.empty()==false) {
            std::vector<float> bb = getTractogramBBox(&tractogram);
            bb.push_back(-0.5);         // This is the 4th, i.e., the DEC dimension
            bb.push_back( 2.5);

            if (voxDim.size()==1) {
                voxDim.push_back(voxDim[0]);
                voxDim.push_back(voxDim[0]);
            }
            voxDim.push_back(1);        // Set DEC pixel dimension to 1

            img.createFromBoundingBox(4, bb, voxDim, false);
        }

    
        Tractogram2ImageMapper<float> gridder(&tractogram,&img);

        if (*mask_option) {
            NIBR::Image<int> mask(mask_image);
            if (*mask_label_option) {
                if (!gridder.setMask(&mask,mask_label)) return;
            } else {
                if (!gridder.setMask(&mask))            return;
            }            
        }

        gridder.anisotropicSmoothing(smoothing);

        allocateGrid_4DECsegmentLength(&gridder);
        gridder.run(processor_4DECsegmentLength<float>, outputCompiler_4DECsegmentLength<float>);
        deallocateGrid_4DECsegmentLength(&gridder);

        img.write(out_fname);
        
    }
    
    if (feature=="streamlineCount") {

        NIBR::Image<uint32_t> img;

        // Use template image
        if (template_image!="") {
            NIBR::Image<uint32_t> tempImg(template_image);
            tempImg.readHeader();
            img.createFromTemplate(tempImg,template_upsampleFactor,false);
        }

        // Use bounding box
        if (bbox.empty()==false) {
            std::vector<float> bb(bbox.begin(),  bbox.begin()+6);
            std::vector<float> pd(bbox.begin()+6,bbox.end());
            img.createFromBoundingBox(3,bb,pd,false);
        }

        // Use voxDim
        if (voxDim.empty()==false) {
            std::vector<float> bb = getTractogramBBox(&tractogram);
            img.createFromBoundingBox(3,bb,voxDim,false);
        }
        
        Tractogram2ImageMapper<uint32_t> gridder(&tractogram,&img);
        
        if (*mask_option) {
            NIBR::Image<int> mask(mask_image);
            if (*mask_label_option) {
                if (!gridder.setMask(&mask,mask_label)) return;
            } else {
                if (!gridder.setMask(&mask))            return;
            }            
        }
        
        gridder.anisotropicSmoothing(smoothing);
        gridder.setMapOnce(true);
        allocateGrid_4streamlineCount(&gridder);
        gridder.run(processor_4streamlineCount<uint32_t>, outputCompiler_4streamlineCount<uint32_t>);
        deallocateGrid_4streamlineCount(&gridder);

        img.write(out_fname);

    }

    NIBR::SF::clean();

}


void track2img(CLI::App* app)
{
    // app->formatter(std::make_shared<CustomHelpFormatter>());
 
    const std::string info = "\033[1mREFERENCES\033[0m:"
        "\n\nFor uses without spherical mapping: Calamante, F.; Tournier, J.-D.; Jackson, G. D. & Connelly, A. Track-density imaging (TDI): Super-resolution white matter imaging using whole-brain track-density mapping. NeuroImage, 2010, 53, 1233-1243"
        "\n\nFor uses with spherical mapping: Dhollander, T., Emsell, L., Van Hecke, W., Maes, F., Sunaert, S., Suetens, P. Track Orientation Density Imaging (TODI) and Track Orientation Distribution (TOD) based tractography. NeuroImage, 2014, 94, 312-336";


    setInfo(app,info);

    app->description("maps tractogram features on an image");

    app->add_option("<input tractogram>", inp_fname, "Input tractogram (.vtk, .tck)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("--feature", feature, "Name of output feature. Options are: \"streamlineCount\", \"segmentLength\" or \"DEC_SegmentLength\".");

    app->add_option("--weights", weights, "A binary file for streamline weights. Values should have float datatype.")
        ->check(CLI::ExistingFile);

    app->add_option("--anisotropicSmoothing", smoothing, "Standard deviation of the Gaussian kernel, and computation density for anisotropic smoothing. Default=0 0");

    app->add_option("--SF", sfRes, "Spherical domain detail. This value should be a positive odd integer up to 31.");

    app->add_option("--SF_smoothing", sfSmoothing, "Apply spherical smoothing.");

    app->add_option("--SH", shOrder, "Order of spherical harmonics function. Only even orders are supported.");

    app->add_option("--template,-t", template_image, "Input template image (.nii/.nii.gz)")
        ->check(CLI::ExistingFile);

    app->add_option("--templateUpsample", template_upsampleFactor, "Upsampling factor for template image");

    app->add_option("--bbox,-b", bbox, "Bounding box for the output image. \nOptions are: \n    (i) \"x_min x_max y_min y_max z_min z_max\" (an isotropic voxel dimension of 1 will be assumed.)\n   (ii) \"x_min x_max y_min y_max z_min z_max voxDim\" (an isotropic voxel dimension of voxDim will be assumed.)\n  (iii) \"x_min x_max y_min y_max z_min z_max xVoxDim yVoxDim zVoxDim\" ");

    app->add_option("--voxDim", voxDim, "Voxel dimension of the output image. \nOptions are: \n    (i) a single value that will be used for all dimensions \n   (ii) one value for each three dimensions seperated space");

    mask_option = app->add_option("--mask,", mask_image, "Input mask image (.nii/.nii.gz)")
        ->check(CLI::ExistingFile);
        
    mask_label_option = app->add_option("--maskLabel", mask_label, "Label to use in the mask image");
        
    app->add_option("<output image>", out_fname, "Output image name (.nii, .nii.gz)")
        ->required();
        
    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quite\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");
    
    app->callback(run_track2img);
    
}

