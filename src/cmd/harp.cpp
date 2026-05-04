#include "cmd.h"

namespace CMDARGS_HARP {
    std::string  inp_fname;
    std::string  xact_fname;
    std::string  out_table  = "";

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_HARP;


// We will have 27 pathways in total. 
// The first 5 rules check which part of the brain the streamline enters.
// The rest check which two regions the streamline connects.
#define enters_CSF          0
#define enters_LN_WM        1
#define enters_RN_WM        2
#define enters_CER_WM       3
#define enters_BS           4
#define L1_ends             5
#define LN_GM___LN_GM       6
#define LN_GM___RN_GM       7
#define LN_GM___L_SUB       8
#define LN_GM___R_SUB       9
#define LN_GM___CER_GM      10
#define LN_GM___BS          11
#define RN_GM___RN_GM       12
#define RN_GM___L_SUB       13
#define RN_GM___R_SUB       14
#define RN_GM___CER_GM      15
#define RN_GM___BS          16
#define L_SUB___L_SUB       17
#define L_SUB___R_SUB       18
#define L_SUB___CER_GM      19
#define L_SUB___BS          20
#define R_SUB___R_SUB       21
#define R_SUB___CER_GM      22
#define R_SUB___BS          23
#define CER_GM___CER_GM     24
#define CER_GM___BS         25
#define BS___BS             26
#define NUM_PATHWAYS        27

// We will assign each streamline to one of the 4 courseclasses based on the pathways it matches.
typedef enum {
    L1_implausible,
    L1_plausible,
    L2_implausible,
    L2_plausible
} Streamline_Class_Course;

// We will assign each streamline to one of the 33 detailed classes based on the pathways it matches.
// First 3 are L1 implausibility classes. Note that the rest are subclasses of L1 plausible on the hierarchical ladder.
// The next 10 classes are L2 implausibility classes.
// The rest of the 21 classes are L2 plausible classes, and they depict which two regions the streamline connects.
typedef enum {
    L1_implausible_CSF_entry = 0,
    L1_implausible_WM_termination,
    L1_implausible_CSF_entry_and_WM_termination,
    L2_implausible_trj1,
    L2_implausible_trj2,
    L2_implausible_trj3,
    L2_implausible_trj4,
    L2_implausible_trj5,
    L2_implausible_trj6,
    L2_implausible_trj7,
    L2_implausible_trj8,
    L2_implausible_trj9,
    L2_implausible_trj10,
    L2_LN_GM___LN_GM,
    L2_LN_GM___RN_GM,
    L2_LN_GM___L_SUB,
    L2_LN_GM___R_SUB,
    L2_LN_GM___CER_GM,
    L2_LN_GM___BS,
    L2_RN_GM___RN_GM,
    L2_RN_GM___L_SUB,
    L2_RN_GM___R_SUB,
    L2_RN_GM___CER_GM,
    L2_RN_GM___BS,
    L2_L_SUB___L_SUB,
    L2_L_SUB___R_SUB,
    L2_L_SUB___CER_GM,
    L2_L_SUB___BS,
    L2_R_SUB___R_SUB,
    L2_R_SUB___CER_GM,
    L2_R_SUB___BS,
    L2_CER_GM___CER_GM,
    L2_CER_GM___BS,
    L2_BS___BS
} Streamline_Class_Detailed;

// Helper function to write a boolean matrix to a CSV file
bool write_matrix_to_csv(const std::string& filename, const std::vector<std::vector<bool>>& data) {
    
    // Open the file stream for writing
    std::ofstream file(filename);

    // Check if the file opened successfully
    if (!file.is_open()) {
        disp(MSG_ERROR, "Failed to open file for writing: %s", filename.c_str());
        return false;
    }

    // Iterate over each row (streamline)
    for (size_t i = 0; i < data.size(); ++i) {
        
        // Iterate over each column (pathway) in the current row
        for (size_t j = 0; j < data[i].size(); ++j) {
            
            // Write the boolean value as 1 or 0
            file << (data[i][j] ? "1" : "0");

            // Add a comma after each element except the last one in the row
            if (j < data[i].size() - 1) {
                file << ",";
            }
        }
        // Add a newline character at the end of each row
        file << "\n";
    }

    return true;
}

void run_harp()
{

    parseCommon(numberOfThreads,verbose);
    bool makeTable = (out_table != "");
    if (makeTable && !parseForceOutput(out_table,force)) return;

    // Initialize tractogram
    NIBR::TractogramReader tractogram(inp_fname);
    if (!tractogram.isReady()) {
        disp(MSG_ERROR,"Can't read %s.", inp_fname.c_str());
        return;
    }

    // Initialize xact
    NIBR::Surface combined(xact_fname);

    if (combined.extension != "vtk") { 
        disp(MSG_ERROR,"This function only works with .vtk files.");
        return;
    } 

    combined.readMesh();
    disp(MSG_DETAIL,"xact mesh read");

    auto xact = combined.readField("xact");
    disp(MSG_DETAIL,"xact field read");

    disp(MSG_INFO,"Decomposing xact surfaces");
    Surface LN_WM;   selectVertices(&LN_WM,   &combined, &xact,  1); LN_WM.enablePointCheck(1.0f);
    Surface RN_WM;   selectVertices(&RN_WM,   &combined, &xact,  2); RN_WM.enablePointCheck(1.0f);
    Surface LN_GM;   selectVertices(&LN_GM,   &combined, &xact,  3); LN_GM.enablePointCheck(1.0f);
    Surface RN_GM;   selectVertices(&RN_GM,   &combined, &xact,  4); RN_GM.enablePointCheck(1.0f);
    Surface L_SUB;   selectVertices(&L_SUB,   &combined, &xact,  5); L_SUB.enablePointCheck(1.0f);
    Surface R_SUB;   selectVertices(&R_SUB,   &combined, &xact,  6); R_SUB.enablePointCheck(1.0f);
    Surface CSF;     selectVertices(&CSF,     &combined, &xact,  7); CSF.enablePointCheck(1.0f);
    Surface CER_WM;  selectVertices(&CER_WM,  &combined, &xact,  8); CER_WM.enablePointCheck(1.0f);
    Surface CER_GM;  selectVertices(&CER_GM,  &combined, &xact,  9); CER_GM.enablePointCheck(1.0f);
    Surface BS;      selectVertices(&BS,      &combined, &xact, 10); BS.enablePointCheck(1.0f);

    Surface* endROIs[] = {&LN_GM, &RN_GM, &L_SUB, &R_SUB, &CER_GM, &BS};
    
    combined.clearField(xact);
    disp(MSG_INFO,"Done");
    
    // Let's first create a template rule for convenience. 
    PathwayRule rule;

    // This function below will be used to reset the rule so we don't need to create a new one everytime.
    auto resetRule = [&]() { 
        PathwayRule tmp;
        tmp.surfaceSource              = xact_fname;
        tmp.surfaceUseDim              = surf_useDim_3D;
        tmp.src                        = NIBR::surf_src;
        tmp.surfaceDiscretizationRes   = 1;
        rule = tmp;
    };

    // Prepare the pathways
    std::vector<std::unique_ptr<Pathway>> pathways;

    auto addCheckEntryRule = [&](int ruleNo, const Surface& surf, std::string surfName) 
    {
        auto path = std::make_unique<Pathway>();
        path->enableTracking(false);

        resetRule();
        rule.type    = req_entry;
        rule.surfSrc = new Surface(surf,true); // The pathway will take ownership of the surface and delete it.
        rule.side    = either;
        if (!path->add(rule)) return;

        pathways.push_back(std::move(path));

        if (int(NIBR::VERBOSE()) >= NIBR::VERBOSE_DETAIL) {
            disp(MSG_DETAIL,("enters " + surfName + " path").c_str());
            pathways[ruleNo]->print();
        }
    };

    addCheckEntryRule(enters_CSF,       CSF,    "CSF");     // Pathway #0.
    addCheckEntryRule(enters_LN_WM,     LN_WM,  "LN_WM");   // Pathway #1.
    addCheckEntryRule(enters_RN_WM,     RN_WM,  "RN_WM");   // Pathway #2.
    addCheckEntryRule(enters_CER_WM,    CER_WM, "CER_WM");  // Pathway #3.
    addCheckEntryRule(enters_BS,        BS,     "BS");      // Pathway #4.


    // Streamline classes
    std::vector<Streamline_Class_Course>   courseClass(tractogram.numberOfStreamlines);
    std::vector<Streamline_Class_Detailed> detailedClass(tractogram.numberOfStreamlines);

    // This is the binary output table. It will be of size numberOfStreamlines x number of paths.
    std::vector<std::vector<bool>> out;
    if (makeTable) {
        out.resize(tractogram.numberOfStreamlines);
        for (auto& row : out) {
            row.resize(NUM_PATHWAYS, false);
        }
    }

    // Filter streamline in batches
    int batch_size_max  = 50000;
    int batch_start_idx = 0;

    while (true) {

        // Read the next batch of streamlines
        NIBR::StreamlineBatch input_batch = tractogram.getNextStreamlineBatch(batch_size_max);
        int batch_size = int(input_batch.size());
        if (input_batch.empty()) break;

        // Keep track of each streamline's status regarding each pathway.
        std::vector<std::vector<bool>> batch_stat = std::vector<std::vector<bool>>(input_batch.size(), std::vector<bool>(NUM_PATHWAYS, false));

        // Loop over the all the pathways
        for (int i = 0; i < int(pathways.size()); i++) {
            
            // Apply the current pathway rules. filtered_batch contains [selected streamlines, their indices]
            auto filtered_batch = pathways[i]->apply(input_batch);

            for (const auto& idx : std::get<1>(filtered_batch)) {
                batch_stat[idx][i] = true;
            }
            
        }

        // Loop over end points
        auto checkEndPointConnectivity = [&](const MT::TASK& task) {
            
            float firstPoint[3] = {input_batch[task.no].front()[0] , input_batch[task.no].front()[1], input_batch[task.no].front()[2]};
            float lastPoint[3]  = {input_batch[task.no].back()[0] ,  input_batch[task.no].back()[1],  input_batch[task.no].back()[2]};

            int n = 1;
            for (int i = 0; i < 6; i++) {
                for (int j = i; j < 6; j++) {
                    bool isConnected = (endROIs[i]->isPointInside(firstPoint) && endROIs[j]->isPointInside(lastPoint)) || 
                                       (endROIs[i]->isPointInside(lastPoint)  && endROIs[j]->isPointInside(firstPoint));

                    if (isConnected) {
                        batch_stat[task.no][L1_ends]     = true;
                        batch_stat[task.no][L1_ends + n] = true;
                    }

                    n++;
                }
            }
            
            // All pathways are processed by this point.
            // We can now clear ambiguities and assign the class for this streamline.

            // We start by L1 status. 

            const auto& stat = batch_stat[task.no];

            {
                // L1 implausibility
                bool implausible = false;
                if (stat[enters_CSF])                   {detailedClass[batch_start_idx + task.no] = L1_implausible_CSF_entry;                    implausible = true;}
                if (!stat[L1_ends])                     {detailedClass[batch_start_idx + task.no] = L1_implausible_WM_termination;               implausible = true;}
                if (stat[enters_CSF] && !stat[L1_ends]) {detailedClass[batch_start_idx + task.no] = L1_implausible_CSF_entry_and_WM_termination; implausible = true;}
                
                courseClass[batch_start_idx + task.no] = (implausible) ? L1_implausible : L1_plausible;
            }

            {
                // L2 implausibility
                bool implausible = false;
                

            }
            
            
        };
        MT::MTRUN(batch_size, checkEndPointConnectivity);

        if (makeTable) {
            for (int i = 0; i < batch_size; i++) {
                for (int j = 0; j < NUM_PATHWAYS - 1; j++) {
                    out[batch_start_idx + i][j] = batch_stat[i][j];
                }
            }
        }

        batch_start_idx += batch_size;
        
    }


    // Write the output as a .csv file
    if (makeTable) {
        disp(MSG_INFO, "Writing output table");
        if (write_matrix_to_csv(out_table, out)) {
            disp(MSG_INFO, "Successfully wrote csv file.");
        } else {
            disp(MSG_ERROR, "Failed to write output file.");
        }
    }

    disp(MSG_INFO, "Done.");

    return;

}


void harp(CLI::App* app)
{

    app->description("applies harp");

    app->add_option("<input_tractogram>",    inp_fname,          "Input tractogram (.vtk, .tck, .trk, .trx)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option ("<xact>",               xact_fname,         "Combined xact surface mesh file created prepXact")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option ("--output_table",       out_table,          "A .csv table marking the streamlines and matching pathways");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_harp);
    
}
