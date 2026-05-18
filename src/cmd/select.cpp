#include "cmd.h"
#include <algorithm>

using namespace NIBR;

namespace CMDARGS_SELECT {
    std::string inp_fname;
    std::string out_fname;

    CLI::Option* selectOpt  = NULL;
    CLI::Option* labelOpt   = NULL;
    CLI::Option* randomOpt  = NULL;
    CLI::Option* orderedOpt = NULL;

    std::string select_fname             = "";
    std::vector<std::string> label_list  = std::vector<std::string>();
    int select_random                    = 0;
    std::vector<int> select_ordered      = {0,0};

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_SELECT;

void run_select()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;
    if (!ensureNoTrk(out_fname)) return;

    int optCounter = 0;
    if (*selectOpt)  optCounter++;
    if (*labelOpt)   optCounter++;
    if (*randomOpt)  optCounter++;
    if (*orderedOpt) optCounter++;

    if ( (optCounter==0) || (optCounter>1) ) {
        NIBR::disp(MSG_ERROR, "Need exactly one selection option. Use either \"random\", \"ordered\", \"selection\" or \"label\".");
        return;
    }

    bool trxToTrx = (getFileExtension(inp_fname) == "trx" && getFileExtension(out_fname) == "trx");
    NIBR::TractogramReader tractogram(inp_fname, trxToTrx /*preload*/, trxToTrx /*loadTrxFields*/);
    if (!tractogram.isReady()) return;

    int N = tractogram.numberOfStreamlines;

    std::vector<size_t> select;

    if(*selectOpt){

        FILE *selectFile;
        selectFile = fopen(select_fname.c_str(), "rb");

        if (selectFile == NULL) {
            NIBR::disp(MSG_ERROR, "Can't read \"selection\" file: %s.", select_fname.c_str());
            return;
        }

        float tmp;
        for (int n = 0; n < N; n++) {
            std::fread(&tmp, sizeof(float), 1, selectFile);
            if (tmp > 0)
                select.push_back(n);
        }
        fclose(selectFile);

        writeTractogram(out_fname, &tractogram, select);

    } else if (*labelOpt) {

        if (label_list.size() < 2) {
            NIBR::disp(MSG_ERROR, "Need at least two inputs for \"label\" option. Use --label <file>,<label1>,<label2>,...");
            return;
        }

        std::string label_fname = label_list[0];
        std::vector<uint16_t> labels(label_list.size()-1);
        for (size_t i=1; i<label_list.size(); i++) {
            try {
                int label = std::stoi(label_list[i]);
                if (label < 0 || label > 65535) {
                    NIBR::disp(MSG_ERROR, "Label value out of range (0-65535): %s.", label_list[i].c_str());
                    return;
                }
                labels[i-1] = static_cast<uint16_t>(label);
            } catch (const std::exception& e) {
                NIBR::disp(MSG_ERROR, "Invalid label value: %s. Must be an integer.", label_list[i].c_str());
                return;
            }
        }

        // Read the binary label file
        std::vector<uint16_t> streamline_labels(N);
        FILE *labelFile = fopen(label_fname.c_str(), "rb");
        if (labelFile == NULL) {
            NIBR::disp(MSG_ERROR, "Can't read \"labels\" file: %s.", label_fname.c_str());
            return;
        }
        size_t read_count = fread(streamline_labels.data(), sizeof(uint16_t), N, labelFile);
        fclose(labelFile);
        if (read_count != static_cast<size_t>(N)) {
            NIBR::disp(MSG_ERROR, "Label file size does not match number of streamlines. Expected %d labels, got %d.", N, read_count);
            return;
        }

        // Select streamlines with matching labels
        for (int n = 0; n < N; n++) {
            if (std::find(labels.begin(), labels.end(), streamline_labels[n]) != labels.end()) {
                select.push_back(n);
            }
        }

        writeTractogram(out_fname, &tractogram, select);

    } else if(*randomOpt){

        if (select_random<0) select_random = 0;
        if (select_random>N) select_random = N;

        RandomDoer r;

        std::vector<size_t> allInd;
        allInd.resize(N);
        for (auto i=0; i<N; i++)
            allInd[i] = i;

        std::shuffle(std::begin(allInd), std::end(allInd), r.getGen());
        select.insert(select.begin(),allInd.begin(),allInd.begin()+select_random);
        std::sort(select.begin(), select.end()); // match sorted order of writeTractogram

        NIBR::writeTractogram(out_fname, &tractogram, select);

    } else {

        if (select_ordered[0] <= 0) {
            NIBR::disp(MSG_WARN, "Index can't be 0 or smaller. We will use 1 instead.");
            select_ordered[0] = 1;
        }

        if (select_ordered[1] <= 0) {
            NIBR::disp(MSG_WARN, "Index can't be 0 or smaller. We will use 1 instead.");
            select_ordered[1] = 1;
        }

        if (select_ordered[0]  > N) {
            NIBR::disp(MSG_WARN, "Index can't be larger than %d. We will use %d instead.", N, N);
            select_ordered[0] = N;
        }

        if (select_ordered[1]  > N) {
            NIBR::disp(MSG_WARN, "Index can't be larger than %d. We will use %d instead.", N, N);
            select_ordered[1] = N;
        }

        for (int i = select_ordered[0]; i < (select_ordered[1]+1); i++)
            select.push_back(i-1);

        NIBR::writeTractogram(out_fname, &tractogram, select);

    }

    if (select.size() == 0)
        disp(MSG_INFO,"Output is empty.");
    else if (select.size() == 1)
        disp(MSG_INFO,"1 streamline is written.");
    else
        disp(MSG_INFO,"%d streamlines are written.", select.size());

    return;
}
  
  
void select(CLI::App* app) 
{  

    app->description("selects streamlines from a tractogram");

    app->add_option("<input_tractogram>",           inp_fname,          "Input tractogram (.trx, .vtk, .tck, .trk)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<output_tractogram>",          out_fname,          "Output tractogram (.trx, .vtk, .tck)")
        ->required();    
    
    selectOpt = app->add_option("--selection, -s",  select_fname,       "File with binary values that mark selected streamlines with 1 and others with 0");    
    labelOpt  = app->add_option("--label, -l",      label_list,         "Select streamlines given a binary (uint16) file and a list of labels, e.g. labels.uint16,10,223,3232")->delimiter(',');
    randomOpt = app->add_option("--random, -r",     select_random,      "Random tractogram file creating. One input required, total count for random lines")->expected(1);
    orderedOpt= app->add_option("--ordered, -o",    select_ordered,     "Ordered tractogram file creating. Two input required, begin and end index")->expected(2)->delimiter(' ');

    app->add_option("--numberOfThreads, -n",        numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",                verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",                    force,              "Force overwriting of existing file");

    app->callback(run_select);
    
} 

