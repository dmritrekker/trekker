#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_TRACK2SURFACE {

    std::string inp_fname;
    std::string inp_surface;
    std::string out_surface;

    std::string feature;
    std::string field_name;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;

}

using namespace CMDARGS_TRACK2SURFACE;

void run_track2surf()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_surface,force)) return;

    if ((feature!="streamlineDensity") && (feature!="streamlineCount") && (feature!="contactAngle") && (feature!="contactDirection")) {
        std::cout << "Feature can be \"streamlineDensity\", \"streamlineCount\", \"contactAngle\" and \"contactDirection\"" << std::endl << std::flush;
        return;
    }
    
    // Initialize tractogram
    if(!ensureVTKorTCK(inp_fname)) return;
    NIBR::TractogramReader* tractogram = new NIBR::TractogramReader();
    tractogram->initReader(inp_fname);
    
    if (tractogram->numberOfStreamlines<1) {
        std::cout << "Empty tractogram" << std::endl;
        std::cout << "0 streamlines are written."  << std::endl << std::flush;
        return;
    }
    
    // Read surface mesh
    if(!ensureVTK(out_surface)) return;

    NIBR::Surface* surf = new NIBR::Surface();
    surf->readHeader(inp_surface);

    if ((surf->extension != "vtk") && (surf->extension != "gii")) {
        disp(MSG_ERROR,"Unsupported file format: %s (only vtk/gii are supported)", inp_surface.c_str());
        return;
    }
    
    // Check if this field already exists in the input surface mesh
    int fieldId = -1;
    std::vector<NIBR::SurfaceField> tmpFields = surf->findFields();
    for (size_t i=0; i<tmpFields.size(); i++) {
        if ((tmpFields[i].name == ("vertex_" + field_name)) ||  (tmpFields[i].name == ("face_" + field_name)) ) {
            fieldId = i;
        }
    }
    
    if ((force==false) && (fieldId>-1)) {
        std::cout << "A field with name \"" << field_name << "\" already exists in the input surface mesh. Use --force or -f to overwrite." << std::endl<< std::flush;
        return;
    }
    
    surf->readMesh();
    surf->readFields();
    surf->deleteField("vertex_" + field_name);
    surf->deleteField("face_"   + field_name);

    surf->calcNormalsOfFaces();
    surf->calcCentersOfFaces();
    surf->calcTriangleVectors();
    surf->getNeighboringFaces();

    // Create tractogram to surface map
    std::vector<std::vector<streamline2faceMap>> mapping;    
    tractogram2surfaceMapper(tractogram, surf, mapping, true);
    delete tractogram;

    // Prepare and write selected feature on surface

    if (feature=="streamlineDensity") {
        
        float** fdata = new float*[surf->nf];
        for (int n=0; n<surf->nf; n++) {
            fdata[n]    = new float[1];
            fdata[n][0] = float(mapping[n].size())/surf->areasOfFaces[n];
        }
        
        NIBR::SurfaceField field;
        field.name      = "face_" + field_name;;
        field.owner     = NIBR::FACE;
        field.datatype  = "float";
        field.fdata     = fdata;
        field.idata     = NULL;
        field.dimension = 1;
        surf->fields.push_back(field);
        
        NIBR::SurfaceField vfield = NIBR::convert2VertField(surf, &field);
        vfield.name = "vertex_" + field_name;
        surf->fields.push_back(vfield);
        
    }




    if (feature=="streamlineCount") {
        
        float** fdata = new float*[surf->nf];
        for (int n=0; n<surf->nf; n++) {
            fdata[n]    = new float[1];
            fdata[n][0] = mapping[n].size();
        }
        
        NIBR::SurfaceField field;
        field.name      = "face_" + field_name;;
        field.owner     = NIBR::FACE;
        field.datatype  = "float";
        field.fdata     = fdata;
        field.idata     = NULL;
        field.dimension = 1;
        surf->fields.push_back(field);
        
        NIBR::SurfaceField vfield = NIBR::convert2VertField(surf, &field);
        vfield.name = "vertex_" + field_name;
        surf->fields.push_back(vfield);
        
    }
    
    if (feature=="contactAngle") {
        
        float** fdata = new float*[surf->nf];
        for (int n=0; n<surf->nf; n++) {
            fdata[n]    = new float[1];
            fdata[n][0] = 0;

            for (auto indAng : mapping[n])
                fdata[n][0] += indAng.angle;

            if (mapping[n].size()>0)
                fdata[n][0] /= float(mapping[n].size());

        }
        
        NIBR::SurfaceField field;
        field.name      = "face_" + field_name;
        field.owner     = NIBR::FACE;
        field.datatype  = "float";
        field.fdata     = fdata;
        field.idata     = NULL;
        field.dimension = 1;
        surf->fields.push_back(field);
        
        
        NIBR::SurfaceField vfield = NIBR::convert2VertField(surf, &field);
        vfield.name = "vertex_" + field_name;
        surf->fields.push_back(vfield);
        
    }
    
    if (feature=="contactDirection") {
        
        float** fdata             = new float*[surf->nf];
        float*  segmentDirPerFace = new float[surf->nf];
        float   maxDirMag         = 0;
        
        for (int n=0; n<surf->nf; n++) {
            fdata[n]    = new float[3];
            fdata[n][0] = 0;
            fdata[n][1] = 0;
            fdata[n][2] = 0;
            segmentDirPerFace[n]   = 0;

            for (auto indAng : mapping[n]) {
                fdata[n][0] += std::fabs(indAng.dir[0]);
                fdata[n][1] += std::fabs(indAng.dir[1]);
                fdata[n][2] += std::fabs(indAng.dir[2]);
            }

            fdata[n][0] /= surf->areasOfFaces[n];
            fdata[n][1] /= surf->areasOfFaces[n];
            fdata[n][2] /= surf->areasOfFaces[n];

            if (mapping[n].size()>0) {
                segmentDirPerFace[n] = norm(fdata[n]);
            }

            if (segmentDirPerFace[n]>maxDirMag)
                maxDirMag = segmentDirPerFace[n];

        }
        
        for (int n=0; n<surf->nf; n++) {
            fdata[n][0] /= maxDirMag;
            fdata[n][1] /= maxDirMag;
            fdata[n][2] /= maxDirMag;
        }
        
        NIBR::SurfaceField field;
        field.name      = "face_" + field_name;
        field.owner     = NIBR::FACE;
        field.datatype  = "float";
        field.fdata     = fdata;
        field.idata     = NULL;
        field.dimension = 3;
        surf->fields.push_back(field);
        

        NIBR::SurfaceField vfield = NIBR::convert2VertField(surf, &field);
        vfield.name = "vertex_" + field_name;
        surf->fields.push_back(vfield);

        delete[] segmentDirPerFace;
        
    }

    
    // Write output
    surf->write(out_surface);
    delete surf;

}


void track2surf(CLI::App* app)
{

    app->description("maps tractogram features on a surface");

    app->add_option("<input_tractogram>", inp_fname, "Input tractogram (.vtk, .tck)")
        ->required()
        ->check(CLI::ExistingFile);
    
    app->add_option("<input_surface_mesh>", inp_surface, "Input surface mesh (.vtk,.gii)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<output_surface_mesh>", out_surface, "Output surface mesh (.vtk)")
        ->required();
        
    app->add_option("<field_name>", field_name, "Field name to use when writing the feature on the surface mesh")
        ->required();
        
    app->add_option("--feature", feature, "Name of output feature. Options are: \"streamlineDensity\", \"streamlineCount\", \"contactAngle\" and \"contactDirection\".")
        ->required();

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_track2surf);

}
