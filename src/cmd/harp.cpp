#include "cmd.h"

namespace CMDARGS_HARP {
    std::string  inp_fname;
    std::string  xact_fname;
    std::string  out_labels         = "";
    std::string  out_course_labels  = "";
    std::string  out_counts         = "";

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
    L1_implausible = 1,
    L1_plausible,
    L2_implausible,
    L2_plausible
} Streamline_Class_Course;

// We will assign each streamline to one of the 34 detailed classes based on the pathways it matches.
// First 21 are L2 plausible classes, i.e. end-to-end connections. 
// The next 10 classes are L2 implausible trajectories.
// The last 3 are L1 implausible classes.
typedef enum {
    L2_LN_GM___LN_GM = 1,
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
    L2_BS___BS,
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
    L1_implausible_CSF_entry_and_WM_termination,
    L1_implausible_CSF_entry,
    L1_implausible_WM_termination,
} Streamline_Class_Detailed;

// Logger
std::atomic<std::size_t> log_L1_implausible{0};
std::atomic<std::size_t> log_L1_plausible{0};
std::atomic<std::size_t> log_L2_implausible{0};
std::atomic<std::size_t> log_L2_plausible{0};

std::atomic<std::size_t> log_L1_implausible_CSF_entry_and_WM_termination{0};
std::atomic<std::size_t> log_L1_implausible_CSF_entry{0};
std::atomic<std::size_t> log_L1_implausible_WM_termination{0};

std::atomic<std::size_t> log_L2_implausible_trj1{0};
std::atomic<std::size_t> log_L2_implausible_trj2{0};
std::atomic<std::size_t> log_L2_implausible_trj3{0};
std::atomic<std::size_t> log_L2_implausible_trj4{0};
std::atomic<std::size_t> log_L2_implausible_trj5{0};
std::atomic<std::size_t> log_L2_implausible_trj6{0};
std::atomic<std::size_t> log_L2_implausible_trj7{0};
std::atomic<std::size_t> log_L2_implausible_trj8{0};
std::atomic<std::size_t> log_L2_implausible_trj9{0};
std::atomic<std::size_t> log_L2_implausible_trj10{0};

std::atomic<std::size_t> log_L2_LN_GM___LN_GM{0};
std::atomic<std::size_t> log_L2_LN_GM___RN_GM{0};
std::atomic<std::size_t> log_L2_LN_GM___L_SUB{0};
std::atomic<std::size_t> log_L2_LN_GM___R_SUB{0};
std::atomic<std::size_t> log_L2_LN_GM___CER_GM{0};
std::atomic<std::size_t> log_L2_LN_GM___BS{0};
std::atomic<std::size_t> log_L2_RN_GM___RN_GM{0};
std::atomic<std::size_t> log_L2_RN_GM___L_SUB{0};
std::atomic<std::size_t> log_L2_RN_GM___R_SUB{0};
std::atomic<std::size_t> log_L2_RN_GM___CER_GM{0};
std::atomic<std::size_t> log_L2_RN_GM___BS{0};
std::atomic<std::size_t> log_L2_L_SUB___L_SUB{0};
std::atomic<std::size_t> log_L2_L_SUB___R_SUB{0};
std::atomic<std::size_t> log_L2_L_SUB___CER_GM{0};
std::atomic<std::size_t> log_L2_L_SUB___BS{0};
std::atomic<std::size_t> log_L2_R_SUB___R_SUB{0};
std::atomic<std::size_t> log_L2_R_SUB___CER_GM{0};
std::atomic<std::size_t> log_L2_R_SUB___BS{0};
std::atomic<std::size_t> log_L2_CER_GM___CER_GM{0};
std::atomic<std::size_t> log_L2_CER_GM___BS{0};
std::atomic<std::size_t> log_L2_BS___BS{0};

bool writeBinaryFile(const std::string& filename, const std::vector<uint16_t>& data) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        return false;
    }
    outFile.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(uint16_t));
    return outFile.good();
}

bool writeCSVFile(const std::string& filename, const std::vector<std::pair<std::string, std::size_t>>& rows) {
    std::ofstream outFile(filename);
    if (!outFile) {
        return false;
    }
    for (const auto& row : rows) {
        outFile << row.first << "," << row.second << "\n";
    }
    return outFile.good();
}

void run_harp()
{

    parseCommon(numberOfThreads,verbose);
    parseForceOutput(out_labels,force);
    bool writeCourseLabels = (out_course_labels != "");
    bool writeCounts       = (out_counts != "");
    if (writeCourseLabels && !parseForceOutput(out_course_labels,force)) return;
    if (writeCounts       && !parseForceOutput(out_counts,force)) return;

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

    addCheckEntryRule(enters_CSF,    CSF,    "CSF");
    addCheckEntryRule(enters_LN_WM,  LN_WM,  "LN_WM");
    addCheckEntryRule(enters_RN_WM,  RN_WM,  "RN_WM");
    addCheckEntryRule(enters_CER_WM, CER_WM, "CER_WM");
    addCheckEntryRule(enters_BS,     BS,     "BS");

    // Streamline classes
    std::vector<uint16_t> courseClass(tractogram.numberOfStreamlines);
    std::vector<uint16_t> detailedClass(tractogram.numberOfStreamlines);

    // Helper function for printing progress
    auto printProgress = [&]()
{
        // Print out the statistics
        std::size_t total_analyzed = log_L1_implausible.load() + log_L2_implausible.load() + log_L2_plausible.load();

        std::cout << "\033[0J";

        std::cout << std::endl;
        disp(MSG_INFO, "Total number of analyzed streamlines / all streamlines: %zu / %zu", total_analyzed, tractogram.numberOfStreamlines);
        std::cout << std::endl;
        disp(MSG_INFO, "L1 implausible: %zu", log_L1_implausible.load());
        disp(MSG_INFO, "L1 plausible:   %zu", log_L1_plausible.load());
        disp(MSG_INFO, "L2 implausible: %zu", log_L2_implausible.load());
        disp(MSG_INFO, "L2 plausible:   %zu", log_L2_plausible.load());
        std::cout << std::endl;
        disp(MSG_INFO, "L1 implausible - CSF entry and WM termination:  %zu", log_L1_implausible_CSF_entry_and_WM_termination.load());
        disp(MSG_INFO, "L1 implausible - CSF entry:                     %zu", log_L1_implausible_CSF_entry.load());
        disp(MSG_INFO, "L1 implausible - WM termination:                %zu", log_L1_implausible_WM_termination.load());
        std::cout << std::endl;
        disp(MSG_INFO, "L2 implausible - trj1:  %zu", log_L2_implausible_trj1.load());
        disp(MSG_INFO, "L2 implausible - trj2:  %zu", log_L2_implausible_trj2.load());
        disp(MSG_INFO, "L2 implausible - trj3:  %zu", log_L2_implausible_trj3.load());
        disp(MSG_INFO, "L2 implausible - trj4:  %zu", log_L2_implausible_trj4.load());
        disp(MSG_INFO, "L2 implausible - trj5:  %zu", log_L2_implausible_trj5.load());
        disp(MSG_INFO, "L2 implausible - trj6:  %zu", log_L2_implausible_trj6.load());
        disp(MSG_INFO, "L2 implausible - trj7:  %zu", log_L2_implausible_trj7.load());
        disp(MSG_INFO, "L2 implausible - trj8:  %zu", log_L2_implausible_trj8.load());
        disp(MSG_INFO, "L2 implausible - trj9:  %zu", log_L2_implausible_trj9.load());
        disp(MSG_INFO, "L2 implausible - trj10: %zu", log_L2_implausible_trj10.load());
        std::cout << std::endl;
        disp(MSG_INFO, "L2 plausible - LN_GM___LN_GM:   %zu", log_L2_LN_GM___LN_GM.load());
        disp(MSG_INFO, "L2 plausible - LN_GM___RN_GM:   %zu", log_L2_LN_GM___RN_GM.load());
        disp(MSG_INFO, "L2 plausible - LN_GM___L_SUB:   %zu", log_L2_LN_GM___L_SUB.load());
        disp(MSG_INFO, "L2 plausible - LN_GM___R_SUB:   %zu", log_L2_LN_GM___R_SUB.load());
        disp(MSG_INFO, "L2 plausible - LN_GM___CER_GM:  %zu", log_L2_LN_GM___CER_GM.load());
        disp(MSG_INFO, "L2 plausible - LN_GM___BS:      %zu", log_L2_LN_GM___BS.load());
        disp(MSG_INFO, "L2 plausible - RN_GM___RN_GM:   %zu", log_L2_RN_GM___RN_GM.load());
        disp(MSG_INFO, "L2 plausible - RN_GM___L_SUB:   %zu", log_L2_RN_GM___L_SUB.load());
        disp(MSG_INFO, "L2 plausible - RN_GM___R_SUB:   %zu", log_L2_RN_GM___R_SUB.load());
        disp(MSG_INFO, "L2 plausible - RN_GM___CER_GM:  %zu", log_L2_RN_GM___CER_GM.load());
        disp(MSG_INFO, "L2 plausible - RN_GM___BS:      %zu", log_L2_RN_GM___BS.load());
        disp(MSG_INFO, "L2 plausible - L_SUB___L_SUB:   %zu", log_L2_L_SUB___L_SUB.load());
        disp(MSG_INFO, "L2 plausible - L_SUB___R_SUB:   %zu", log_L2_L_SUB___R_SUB.load());
        disp(MSG_INFO, "L2 plausible - L_SUB___CER_GM:  %zu", log_L2_L_SUB___CER_GM.load());
        disp(MSG_INFO, "L2 plausible - L_SUB___BS:      %zu", log_L2_L_SUB___BS.load());
        disp(MSG_INFO, "L2 plausible - R_SUB___R_SUB:   %zu", log_L2_R_SUB___R_SUB.load());
        disp(MSG_INFO, "L2 plausible - R_SUB___CER_GM:  %zu", log_L2_R_SUB___CER_GM.load());
        disp(MSG_INFO, "L2 plausible - R_SUB___BS:      %zu", log_L2_R_SUB___BS.load());
        disp(MSG_INFO, "L2 plausible - CER_GM___CER_GM: %zu", log_L2_CER_GM___CER_GM.load());
        disp(MSG_INFO, "L2 plausible - CER_GM___BS:     %zu", log_L2_CER_GM___BS.load());
        disp(MSG_INFO, "L2 plausible - BS___BS:         %zu", log_L2_BS___BS.load());
        std::cout << std::endl;
    };

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

            // Assign end-to-end connectivity while resolving the ambiguities. 
            // A streamline can match multiple pathways because of segmentation errors. 
            // We want it to be assigned to only one class (the first matching endpoint).
            int n = 1;
            for (int i = 0; i < 6; i++) {
                for (int j = i; j < 6; j++) {
                    bool isConnected = (endROIs[i]->isPointInside(firstPoint) && endROIs[j]->isPointInside(lastPoint)) || 
                                       (endROIs[i]->isPointInside(lastPoint)  && endROIs[j]->isPointInside(firstPoint));

                    if (isConnected) {
                        batch_stat[task.no][L1_ends]     = true;
                        batch_stat[task.no][L1_ends + n] = true;
                        break;
                    }

                    n++;
                }
                if (batch_stat[task.no][L1_ends]) break;
            }
            
            // All pathways are processed by this point.
            // We can now clear ambiguities and assign the class for this streamline.
            const auto& stat = batch_stat[task.no];
            
            bool isImplausible = false;

            auto assignImplausibleClass = [&](Streamline_Class_Detailed impl_class) {
                detailedClass[batch_start_idx + task.no] = impl_class; 
                isImplausible     = true;
            };

        
            // L1 class assignment
            {
                if (!isImplausible && stat[enters_CSF] && !stat[L1_ends]) {
                    assignImplausibleClass(L1_implausible_CSF_entry_and_WM_termination);
                    log_L1_implausible_CSF_entry_and_WM_termination.fetch_add(1);
                }

                if (!isImplausible && stat[enters_CSF]) {
                    assignImplausibleClass(L1_implausible_CSF_entry);
                    log_L1_implausible_CSF_entry.fetch_add(1);
                }
                
                if (!isImplausible && !stat[L1_ends]) {
                    assignImplausibleClass(L1_implausible_WM_termination);
                    log_L1_implausible_WM_termination.fetch_add(1);
                }

                if (isImplausible) {
                    courseClass[batch_start_idx + task.no] = L1_implausible;
                    log_L1_implausible.fetch_add(1);
                } else {
                    courseClass[batch_start_idx + task.no] = L1_plausible;
                    log_L1_plausible.fetch_add(1);
                }

            }

            // If L1 plausible, then we assign an L2 class.
            if (!isImplausible) {
                
                if (!isImplausible && stat[RN_GM___RN_GM] && stat[enters_LN_WM]) {
                    assignImplausibleClass(L2_implausible_trj1);
                    log_L2_implausible_trj1.fetch_add(1);
                }
                if (!isImplausible && stat[LN_GM___LN_GM] && stat[enters_RN_WM]) {
                    assignImplausibleClass(L2_implausible_trj2);
                    log_L2_implausible_trj2.fetch_add(1);
                }
                if (!isImplausible && stat[LN_GM___BS] && stat[enters_RN_WM]) {
                    assignImplausibleClass(L2_implausible_trj3);
                    log_L2_implausible_trj3.fetch_add(1);
                }
                if (!isImplausible && stat[RN_GM___BS] && stat[enters_LN_WM]) {
                    assignImplausibleClass(L2_implausible_trj4);
                    log_L2_implausible_trj4.fetch_add(1);
                }
                if (!isImplausible && stat[RN_GM___RN_GM] && stat[enters_BS]) {
                    assignImplausibleClass(L2_implausible_trj5);
                    log_L2_implausible_trj5.fetch_add(1);
                }
                if (!isImplausible && stat[LN_GM___LN_GM] && stat[enters_BS]) {
                    assignImplausibleClass(L2_implausible_trj6);
                    log_L2_implausible_trj6.fetch_add(1);
                }
                if (!isImplausible && stat[LN_GM___RN_GM] && stat[enters_BS]) {
                    assignImplausibleClass(L2_implausible_trj7);
                    log_L2_implausible_trj7.fetch_add(1);
                }
                if (!isImplausible && stat[BS___BS] && stat[enters_LN_WM]) {
                    assignImplausibleClass(L2_implausible_trj8);
                    log_L2_implausible_trj8.fetch_add(1);
                }
                if (!isImplausible && stat[BS___BS] && stat[enters_RN_WM]) {
                    assignImplausibleClass(L2_implausible_trj9);
                    log_L2_implausible_trj9.fetch_add(1);
                }
                if (!isImplausible && stat[BS___BS] && stat[enters_CER_WM]) {
                    assignImplausibleClass(L2_implausible_trj10);
                    log_L2_implausible_trj10.fetch_add(1);
                }

                // The rest of the classes are mutually exclusive.
                if (isImplausible) {
                    courseClass[batch_start_idx + task.no] = L2_implausible;
                    log_L2_implausible.fetch_add(1);
                } else {
                    courseClass[batch_start_idx + task.no] = L2_plausible;
                    log_L2_plausible.fetch_add(1);

                    // Assigning detailed class for L2 plausible streamlines.
                    for (int i = LN_GM___LN_GM; i < NUM_PATHWAYS; i++) {
                        if (stat[i]) {
                            switch (i) {
                                case LN_GM___LN_GM:   detailedClass[batch_start_idx + task.no] = L2_LN_GM___LN_GM;   log_L2_LN_GM___LN_GM.fetch_add(1);   break;
                                case LN_GM___RN_GM:   detailedClass[batch_start_idx + task.no] = L2_LN_GM___RN_GM;   log_L2_LN_GM___RN_GM.fetch_add(1);   break;
                                case LN_GM___L_SUB:   detailedClass[batch_start_idx + task.no] = L2_LN_GM___L_SUB;   log_L2_LN_GM___L_SUB.fetch_add(1);   break;
                                case LN_GM___R_SUB:   detailedClass[batch_start_idx + task.no] = L2_LN_GM___R_SUB;   log_L2_LN_GM___R_SUB.fetch_add(1);   break;
                                case LN_GM___CER_GM:  detailedClass[batch_start_idx + task.no] = L2_LN_GM___CER_GM;  log_L2_LN_GM___CER_GM.fetch_add(1);  break;
                                case LN_GM___BS:      detailedClass[batch_start_idx + task.no] = L2_LN_GM___BS;      log_L2_LN_GM___BS.fetch_add(1);      break;
                                case RN_GM___RN_GM:   detailedClass[batch_start_idx + task.no] = L2_RN_GM___RN_GM;   log_L2_RN_GM___RN_GM.fetch_add(1);   break;
                                case RN_GM___L_SUB:   detailedClass[batch_start_idx + task.no] = L2_RN_GM___L_SUB;   log_L2_RN_GM___L_SUB.fetch_add(1);   break;
                                case RN_GM___R_SUB:   detailedClass[batch_start_idx + task.no] = L2_RN_GM___R_SUB;   log_L2_RN_GM___R_SUB.fetch_add(1);   break;
                                case RN_GM___CER_GM:  detailedClass[batch_start_idx + task.no] = L2_RN_GM___CER_GM;  log_L2_RN_GM___CER_GM.fetch_add(1);  break;
                                case RN_GM___BS:      detailedClass[batch_start_idx + task.no] = L2_RN_GM___BS;      log_L2_RN_GM___BS.fetch_add(1);      break;
                                case L_SUB___L_SUB:   detailedClass[batch_start_idx + task.no] = L2_L_SUB___L_SUB;   log_L2_L_SUB___L_SUB.fetch_add(1);   break;
                                case L_SUB___R_SUB:   detailedClass[batch_start_idx + task.no] = L2_L_SUB___R_SUB;   log_L2_L_SUB___R_SUB.fetch_add(1);   break;
                                case L_SUB___CER_GM:  detailedClass[batch_start_idx + task.no] = L2_L_SUB___CER_GM;  log_L2_L_SUB___CER_GM.fetch_add(1);  break;
                                case L_SUB___BS:      detailedClass[batch_start_idx + task.no] = L2_L_SUB___BS;      log_L2_L_SUB___BS.fetch_add(1);      break;
                                case R_SUB___R_SUB:   detailedClass[batch_start_idx + task.no] = L2_R_SUB___R_SUB;   log_L2_R_SUB___R_SUB.fetch_add(1);   break;
                                case R_SUB___CER_GM:  detailedClass[batch_start_idx + task.no] = L2_R_SUB___CER_GM;  log_L2_R_SUB___CER_GM.fetch_add(1);  break;
                                case R_SUB___BS:      detailedClass[batch_start_idx + task.no] = L2_R_SUB___BS;      log_L2_R_SUB___BS.fetch_add(1);      break;
                                case CER_GM___CER_GM: detailedClass[batch_start_idx + task.no] = L2_CER_GM___CER_GM; log_L2_CER_GM___CER_GM.fetch_add(1); break;
                                case CER_GM___BS:     detailedClass[batch_start_idx + task.no] = L2_CER_GM___BS;     log_L2_CER_GM___BS.fetch_add(1);     break;
                                case BS___BS:         detailedClass[batch_start_idx + task.no] = L2_BS___BS;         log_L2_BS___BS.fetch_add(1);         break;
                            }
                        }
                    }
                }

            }
            
            
        };
        MT::MTRUN(batch_size, checkEndPointConnectivity);

        printProgress();
        std::cout << "\033[45F";

        batch_start_idx += batch_size;
        
    }

    printProgress();

    // Write the detailed labels
    if (!writeBinaryFile(out_labels, detailedClass)) {
        disp(MSG_ERROR, "Failed to write the output labels to %s.", out_labels.c_str());
        return;
    }

    // Write the course labels
    if (writeCourseLabels) {
        if (!writeBinaryFile(out_course_labels, courseClass)) {
            disp(MSG_ERROR, "Failed to write the output course labels to %s.", out_course_labels.c_str());
            return;
        }
    }

    // Write the counts CSV file
    if (writeCounts) {
        const std::size_t total_analyzed = log_L1_implausible.load() + log_L2_implausible.load() + log_L2_plausible.load();
        const std::vector<std::pair<std::string, std::size_t>> counts = {
            
            {"Total number of analyzed streamlines", total_analyzed},
            {"Total number of streamlines",          tractogram.numberOfStreamlines},
            
            {"L1 implausible",  log_L1_implausible.load()},
            {"L1 plausible",    log_L1_plausible.load()},
            {"L2 implausible",  log_L2_implausible.load()},
            {"L2 plausible",    log_L2_plausible.load()},
            
            {"L1 implausible - CSF entry and WM termination", log_L1_implausible_CSF_entry_and_WM_termination.load()},
            {"L1 implausible - CSF entry",                    log_L1_implausible_CSF_entry.load()},
            {"L1 implausible - WM termination",               log_L1_implausible_WM_termination.load()},
            
            {"L2 implausible - trj1", log_L2_implausible_trj1.load()},
            {"L2 implausible - trj2", log_L2_implausible_trj2.load()},
            {"L2 implausible - trj3", log_L2_implausible_trj3.load()},
            {"L2 implausible - trj4", log_L2_implausible_trj4.load()},
            {"L2 implausible - trj5", log_L2_implausible_trj5.load()},
            {"L2 implausible - trj6", log_L2_implausible_trj6.load()},
            {"L2 implausible - trj7", log_L2_implausible_trj7.load()},
            {"L2 implausible - trj8", log_L2_implausible_trj8.load()},
            {"L2 implausible - trj9", log_L2_implausible_trj9.load()},
            {"L2 implausible - trj10",log_L2_implausible_trj10.load()},
            
            {"L2 plausible - LN_GM___LN_GM",  log_L2_LN_GM___LN_GM.load()},
            {"L2 plausible - LN_GM___RN_GM",  log_L2_LN_GM___RN_GM.load()},
            {"L2 plausible - LN_GM___L_SUB",  log_L2_LN_GM___L_SUB.load()},
            {"L2 plausible - LN_GM___R_SUB",  log_L2_LN_GM___R_SUB.load()},
            {"L2 plausible - LN_GM___CER_GM", log_L2_LN_GM___CER_GM.load()},
            {"L2 plausible - LN_GM___BS",     log_L2_LN_GM___BS.load()},
            {"L2 plausible - RN_GM___RN_GM",  log_L2_RN_GM___RN_GM.load()},
            {"L2 plausible - RN_GM___L_SUB",  log_L2_RN_GM___L_SUB.load()},
            {"L2 plausible - RN_GM___R_SUB",  log_L2_RN_GM___R_SUB.load()},
            {"L2 plausible - RN_GM___CER_GM", log_L2_RN_GM___CER_GM.load()},
            {"L2 plausible - RN_GM___BS",     log_L2_RN_GM___BS.load()},
            {"L2 plausible - L_SUB___L_SUB",  log_L2_L_SUB___L_SUB.load()},
            {"L2 plausible - L_SUB___R_SUB",  log_L2_L_SUB___R_SUB.load()},
            {"L2 plausible - L_SUB___CER_GM", log_L2_L_SUB___CER_GM.load()},
            {"L2 plausible - L_SUB___BS",     log_L2_L_SUB___BS.load()},
            {"L2 plausible - R_SUB___R_SUB",  log_L2_R_SUB___R_SUB.load()},
            {"L2 plausible - R_SUB___CER_GM", log_L2_R_SUB___CER_GM.load()},
            {"L2 plausible - R_SUB___BS",     log_L2_R_SUB___BS.load()},
            {"L2 plausible - CER_GM___CER_GM",log_L2_CER_GM___CER_GM.load()},
            {"L2 plausible - CER_GM___BS",    log_L2_CER_GM___BS.load()},
            {"L2 plausible - BS___BS",        log_L2_BS___BS.load()},

        };

        if (!writeCSVFile(out_counts, counts)) {
            disp(MSG_ERROR, "Failed to write the output counts to %s.", out_counts.c_str());
            return;
        }
    }

    disp(MSG_INFO, "Done.");

    return;

}


void harp(CLI::App* app)
{

    app->formatter(std::make_shared<CustomHelpFormatter>());
 
    const std::string info = "HARP (Hierarchical Anatomical Refinement of Pathways) labels streamlines based on their end point connectivity and trajectory. It can be used to identify and filter out implausible streamlines that are likely to be false positives, as well as to classify plausible streamlines into different classes based on their end point connectivity patterns."
        "\n\nThe classification is performed in two levels: L1 classification identifies implausible streamlines that either enter cerebrospinal fluid (CSF) or fail to terminate in white matter (WM), while L2 classification further categorizes the L1 plausible streamlines based on their specific end point connectivity patterns, e.g., whether they connect left and right hemisphere GM, or whether they connect to subcortical regions, etc."
        "\n\nThe input tractogram is expected to be in the subject's native space, and the xact surface, which can be generated using the \033[1mprepXact\033[0m command, is expected to be in the same space as the tractogram. The xact surface should have a field named \'xact\' that defines the anatomical regions for each vertex (see the prepXact documentation)."
        "\n\nThe output of HARP is a set of labels for each streamline, which can be used for filtering or further analysis. The default HARP output is a binary file containing the detailed class labels for each streamline, which has a total of 34 classes.\n"
        "\nDetailed class labels:"
        "\n  \033[1m  1\033[0m: L2_LN_GM___LN_GM"
        "\n  \033[1m  2\033[0m: L2_LN_GM___RN_GM"
        "\n  \033[1m  3\033[0m: L2_LN_GM___L_SUB"
        "\n  \033[1m  4\033[0m: L2_LN_GM___R_SUB"
        "\n  \033[1m  5\033[0m: L2_LN_GM___CER_GM"
        "\n  \033[1m  6\033[0m: L2_LN_GM___BS"
        "\n  \033[1m  7\033[0m: L2_RN_GM___RN_GM"
        "\n  \033[1m  8\033[0m: L2_RN_GM___L_SUB"
        "\n  \033[1m  9\033[0m: L2_RN_GM___R_SUB"
        "\n  \033[1m 10\033[0m: L2_RN_GM___CER_GM"
        "\n  \033[1m 11\033[0m: L2_RN_GM___BS"
        "\n  \033[1m 12\033[0m: L2_L_SUB___L_SUB"
        "\n  \033[1m 13\033[0m: L2_L_SUB___R_SUB"
        "\n  \033[1m 14\033[0m: L2_L_SUB___CER_GM"
        "\n  \033[1m 15\033[0m: L2_L_SUB___BS"
        "\n  \033[1m 16\033[0m: L2_R_SUB___R_SUB"
        "\n  \033[1m 17\033[0m: L2_R_SUB___CER_GM"
        "\n  \033[1m 18\033[0m: L2_R_SUB___BS"
        "\n  \033[1m 19\033[0m: L2_CER_GM___CER_GM"
        "\n  \033[1m 20\033[0m: L2_CER_GM___BS"
        "\n  \033[1m 21\033[0m: L2_BS___BS"
        "\n  \033[1m 22\033[0m: L2_implausible_trj1"
        "\n  \033[1m 23\033[0m: L2_implausible_trj2"
        "\n  \033[1m 24\033[0m: L2_implausible_trj3"
        "\n  \033[1m 25\033[0m: L2_implausible_trj4"
        "\n  \033[1m 26\033[0m: L2_implausible_trj5"
        "\n  \033[1m 27\033[0m: L2_implausible_trj6"
        "\n  \033[1m 28\033[0m: L2_implausible_trj7"
        "\n  \033[1m 29\033[0m: L2_implausible_trj8"
        "\n  \033[1m 30\033[0m: L2_implausible_trj9"
        "\n  \033[1m 31\033[0m: L2_implausible_trj10"
        "\n  \033[1m 32\033[0m: L1_implausible_CSF_entry_and_WM_termination"
        "\n  \033[1m 33\033[0m: L1_implausible_CSF_entry"
        "\n  \033[1m 34\033[0m: L1_implausible_WM_termination\n"
        "\nThe first 21 classes are for plausible streamlines and are defined based on their end point connectivity patterns, while the last 13 classes are for implausible streamlines and are defined based on the specific reasons for their implausibility."
        "\n\nThe summary counts for each class can also be written to a CSV file using the \'--out_counts option\'."
        "\n\nHARP can optionally write a separate binary file containing the following course class labels.\n"
        "\nCourse class labels:"
        "\n  \033[1m  1\033[0m: L1 implausible"
        "\n  \033[1m  2\033[0m: L1 plausible"
        "\n  \033[1m  3\033[0m: L2 implausible"
        "\n  \033[1m  4\033[0m: L2 plausible\n"

        "\n\n\033[1mNOTE 1\033[0m:The binary files containing the class labels can be used with the \033[1mselect\033[0m command. For example, to select only L2 plausible streamlines, you can use the command: \n"

        "\n\033[1mtrekker select input_tractogram.trx output_L2_plausible.trx -l course_labels_obtained_with_HARP.bin,4  \033[0m"

        "\n\n\033[1mNOTE 2\033[0m: If you intend to use HARP on whole-brain tractograms obtained with other tools, e.g. MRtrix ACT tractograms, please first use the \033[1mfilter\033[0m command with the \'--xact\' option. This trims the ends of the streamlines to ensure that the end points are correctly classified by HARP, e.g: \n"

        "\n\033[1mtrekker filter input_tractogram_obtained_with_MRtrix_ACT.tck -o xact_compatible_tractogram.trx --xact xact_surface_created_with_prepXact.vtk \033[0m"
        "\n\033[1mtrekker harp xact_compatible_tractogram.trx xact_surface_created_with_prepXact.vtk harp_labels.bin \033[0m"

        "\n\n\033[1mREFERENCES\033[0m:"
        "\n\n[Leserri2026] Leserri S, Rockland K.S., Aydogan D.B., “HARP: Hierarchical anatomical refinement of pathways for whole-brain tractography” (under review) 2026 "
        "\n\n[Leserri2024] Leserri S, Aydogan D.B., “HARP: Hierarchical anatomical refinement of pathways in tractography”, Proceedings of the 27th Annual Meeting of the International Society of Magnetic Resonance in Medicine (ISMRM) 2024, doi: 10.58530/2024/2160.";

    setInfo(app,info);

    app->description("applies harp");

    app->add_option("<input_tractogram>",    inp_fname,          "Input tractogram (.trx, .vtk, .tck, .trk)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option ("<xact>",               xact_fname,         "Combined xact surface mesh file created prepXact (.vtk)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option ("<out_labels>",         out_labels,         "Output uint16 type binary file that contains the detailed class labels for each streamline.")
        ->required();

    app->add_option ("--out_course_labels",  out_course_labels,  "Output uint16 type binary file that contains the course class labels for each streamline.");
    app->add_option ("--out_counts",         out_counts,         "Output CSV file with harp counts and log values.");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_harp);
    
}
