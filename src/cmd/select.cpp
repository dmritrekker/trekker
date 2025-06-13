#include "cmd.h"
#include <algorithm>

using namespace NIBR;

namespace CMDARGS_SELECT {
    std::string inp_fname;
    std::string out_fname;

    CLI::Option* selectOpt  = NULL;
    CLI::Option* randomOpt  = NULL;
    CLI::Option* orderedOpt = NULL;

    std::string select_fname ="";
    int select_random = 0;
    std::vector<int> select_ordered = {0,0};

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_SELECT;


void run_select()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;
    if (!ensureVTKorTCK(out_fname)) return;

    int optCounter = 0;
    if (*selectOpt)  optCounter++;
    if (*randomOpt)  optCounter++;
    if (*orderedOpt) optCounter++;

    if ( (optCounter==0) || (optCounter>1) ) {
        std::cout << "Need one option. Use either \"random\", \"ordered\" or \"selection\"." << std::endl << std::flush;
        return;
    }
    
    NIBR::TractogramReader tractogram(inp_fname);
    if (!tractogram.isReady()) return;

    int N = tractogram.numberOfStreamlines;

    std::vector<size_t> select;

    if(*selectOpt){

        FILE *selectFile;
        selectFile = fopen(select_fname.c_str(), "rb+");

        if (selectFile == NULL) {
            std::cout << "Can't read \"selection\" file: " << select_fname.c_str() << std::endl << std::flush;
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

    app->add_option("<input_tractogram>",           inp_fname,          "Input tractogram (.vtk, .tck, .trk)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<output_tractogram>",          out_fname,          "Output tractogram (.vtk, .tck)")
        ->required();    
    
    selectOpt = app->add_option("--selection, -s",  select_fname,       "File with binary values that mark selected streamlines with 1 and others with 0");    
    randomOpt = app->add_option("--random, -r",     select_random,      "Random tractogram file creating. One input required, total count for random lines")->expected(1);
    orderedOpt= app->add_option("--ordered, -o",    select_ordered,     "Ordered tractogram file creating. Two input required, begin and end index")->expected(2)->delimiter(' ');

    app->add_option("--numberOfThreads, -n",        numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",                verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",                    force,              "Force overwriting of existing file");

    app->callback(run_select);
    
} 

