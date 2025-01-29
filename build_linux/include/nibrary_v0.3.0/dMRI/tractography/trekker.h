#pragma once

#include <vector>
#include "base/nibr.h"

namespace NIBR {

class Trekker {

public:
    
    Trekker();
    Trekker(std::string    a);
    ~Trekker();

    void        run();      // Runs the tracker and populates TRACKER::tractogram
    void        reset();    // Clears TRACKER::tractogram. run() will generate a new TRACKER::tractogram with the same settings.

    // General options
    void        numberOfThreads(int n);
    void        runTimeLimit(int t);
    void        idleTimeLimit(int t);

    // Seeding options
    void        seed_clear();   // All seed options are deleted

    void        seed_count(long count);
    void        seed_density(double density);
    void        seed_trials(int n);

    void        seed_surface_faceDensity(std::string surf_faceDensity_fname);
    void        seed_surface_vertDensity(std::string surf_vertDensity_fname);
    void        seed_surface_fieldDensity(std::string surf_fieldDensity);
    void        seed_surface_density_fileDataType(std::string  densityFileDataType);
    void        seed_surface_useNormForDir(bool useNorm);
    void        seed_surface_useInside(bool useInside);

    // Pathway options
    bool        pathway_addSeed(std::vector<std::string> rule);
    bool        pathway_addRule(std::vector<std::string> rule);
    bool        pathway_remove(int ruleInd);
    
    bool        pathway_minLength(double len);
    bool        pathway_maxLength(double len);
    bool        pathway_stopAtMax(bool q);
    bool        pathway_oneSided (bool q);
    bool        pathway_skipSeed (bool q);
    bool        pathway_inOrder  (bool q);
    // bool        pathway_noEdgeSeed(bool q);
    
    // Algorithm options
    void        algorithm_clear();    // Everything about the algorithm is deleted. All algorithm parameters can always be changed without using this function.

    // FOD options
    void        fod(std::string img_FOD_path);
    void        fodSphere(std::string fod_sphere_path);
    void        fodIsSym(bool fodIsSym);
    void        fodDiscretization(bool fodDiscretization);
    void        orderOfDirections(std::string orderOfDirectionsTextInput);

    // Tracking options
    void        ignoreWeakLinks(double      weakLinkThresh);

    void        writeStepSize(float _writeStepSize);
    void        writeStepSize(std::string paramStr);
    void        stepSize(double      stepSize);
    void        stepSize(std::string paramStr);

    void        minRadiusOfCurvature(double      minRadiusOfCurvature);
    void        minRadiusOfCurvature(std::string paramStr);
    void        minDataSupport(double      minDataSupport);
    void        minDataSupport(std::string paramStr);
    void        dataSupportExponent(double      dataSupportExponent);
    void        dataSupportExponent(std::string paramStr);

    // Sampling options
    void        useBestAtInit(bool     useBestAtInit);    
    void        useLegacySampling(bool useLegacySampling);
    void        samplingQuality(int         samplingQuality);

    void        maxEstInterval(int         maxEstInterval);
    void        maxEstInterval(std::string paramStr);
    void        initMaxEstTrials(int         initMaxEstTrials);
    void        initMaxEstTrials(std::string paramStr);
    void        propMaxEstTrials(int         propMaxEstTrials);
    void        propMaxEstTrials(std::string paramStr);
    void        maxSamplingPerStep(int         triesPerRejectionSampling);
    void        maxSamplingPerStep(std::string paramStr);
    
    // Probe options
    void        probeCount(int         probeCount);
    void        probeCount(std::string paramStr);
    void        probeRadius(double      probeRadius);
    void        probeRadius(std::string paramStr);
    void        probeLength(double      probeLength);
    void        probeLength(std::string paramStr);
    void        probeQuality(int         probeQuality);
    void        probeQuality(std::string paramStr);

    // Parameter mask image
    void        paramImgMask(std::string param_mask_fname);

    // Output options
    void        saveSeedIndex (bool q);
    
    
};

}
