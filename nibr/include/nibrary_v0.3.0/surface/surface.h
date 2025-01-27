#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <assert.h>

#include "base/nibr.h"
#include "image/image.h"
#include "math/core.h"

extern "C" {
#include "gifticlib/gifti_io.h"
}

#undef DT_UNKNOWN
#undef PI

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wstringop-overread"
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wpedantic"
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wunused-local-typedefs"
#pragma clang diagnostic ignored "-Wsign-compare"
#pragma clang diagnostic ignored "-Warray-bounds"
#pragma clang diagnostic ignored "-Wtype-limits"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wreorder"
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wuninitialized"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-result"
#pragma clang diagnostic ignored "-Wcomment"
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#endif

#include <igl/AABB.h>
#include <igl/fast_winding_number.h>

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <Eigen/Core>
#include <Eigen/Dense>

namespace NIBR
{
    // This is not nice but NIBR::PI has to be redefined because compiler does not like it together with igl::PI
    #define PI 		   3.14159265358979323846264338327950288419716939937510582

    // We assume:
    // - the normals are pointing outwards,
    // - the edges do not belong to the surface, i.e. dist != 0.0f,
    // - surfaces have a border containing points, p ∈ (0.0 SURFTHICKNESS],
    // - points within the border are inside the surface.
    // i.e. all points within the surface have distance > 0.0f
    #define SURFTHICKNESS    0.000001

    #define OUTSIDE          0
    #define INSIDE           1
    #define BOUNDARY         2

    // The following are the voxel values when white matter is mapped to image
    #define WM_DEEP          1  // deep white matter
    #define WM_SUPERFICIAL   2  // superficial white matter
    #define WM_SULCAL_FUNDUS 3
    #define WM_SULCAL_WALL   4
    #define WM_GYRAL_CROWN   5
    #define WM_BOUNDARY      6
    
    typedef enum
    {
        FACE,
        VERTEX,
        NOTSET
    } SurfaceFieldOwnerType;

    typedef enum
    {
        OPENORCLOSEDUNSET,
        CLOSED,
        OPEN,
        OPENANDCLOSED
    } OpenOrClosed;

    typedef enum
    {
        MANIFOLD,
        NOTMANIFOLD,
        MANIFOLDORNOT
    } ManifoldOrNot;

    struct SurfaceField
    {
        SurfaceFieldOwnerType owner{NOTSET};
        std::string name{""};
        std::string datatype{""};
        int dimension{0};
        float **fdata{NULL}; // For FACE data, data is nf x DataDimensions. For vertex data, it is nv x DataDimensions
        int **idata{NULL};
    };

    class Surface
    {

    public:
        Surface();
        Surface(std::string _filePath);
        Surface(const Surface &obj);
        Surface(const Surface &obj,bool copyMeshOnly);
        Surface(std::vector<std::vector<float>>& vertexList, std::vector<std::vector<int>>& faceList);
        ~Surface();
        void printInfo();

        bool readHeader(std::string _filePath);        
        bool readMesh();
        void init();
        void clear();
        void reset();
        bool write(std::string _filename);
        
        // TODO: These are only supported for VTK files at the moment. Gifti data fields are not currently processed.
        bool readFields();
        SurfaceField readField(std::string fieldName);
        void deleteField(std::string fieldName);
        SurfaceField copyField(std::string fieldName);
        SurfaceField copyField(SurfaceField& field);
        void clearField(SurfaceField& field);
        void convert2FaceField(SurfaceField& field);
        void convert2VertField(SurfaceField& field);
        void convert2IntField(SurfaceField& field);
        void convert2FloatField(SurfaceField& field);
        std::vector<float> readFloatFieldData(SurfaceField& field,int d);
        std::vector<int>   readIntFieldData  (SurfaceField& field,int d);
        SurfaceField makeVertField(const std::string& name, const std::vector<std::vector<int>>&   idata);
        SurfaceField makeFaceField(const std::string& name, const std::vector<std::vector<int>>&   idata);
        SurfaceField makeVertField(const std::string& name, const std::vector<std::vector<float>>& fdata);
        SurfaceField makeFaceField(const std::string& name, const std::vector<std::vector<float>>& fdata);
        SurfaceField makeVertField(const std::string& name, const std::vector<int>&   idata);
        SurfaceField makeFaceField(const std::string& name, const std::vector<int>&   idata);
        SurfaceField makeVertField(const std::string& name, const std::vector<float>& fdata);
        SurfaceField makeFaceField(const std::string& name, const std::vector<float>& fdata);

        void calcCentersOfFaces();
        void calcNormalsOfFaces();
        void flipNormalsOfFaces();
        void calcAreasOfFaces();
        void calcArea();
        void calcVolume();
        void getNeighboringVertices();
        void getNeighboringFaces();
        void getConnectedComponents();
        void getClosedAndOpenComponents();
        void calcNormalsOfVertices();
        void calcTriangleVectors();
        void calcGaussianCurvature();
        void calcMeanCurvature();
        bool isManifold();
        bool isClosed();
        std::vector<double> calcAreasOfConnectedComponents();
        std::vector<double> calcVolumesOfConnectedComponents();
        std::vector<double> calcAreasOfHoles();

        void toEigen();
        void fromEigen();
        void prepIglAABBTree();

        double squaredDistToPoint(float *p);
        double squaredDistToPoint(float *p, int& faceInd);
        double squaredDistToPoint(float *p, int& faceInd, float* closestPoint);
        double distToPoint(float *p);
        double distToPoint(float *p, int& faceInd);
        double distToPoint(float *p, int& faceInd, float* closestPoint);

        // Enables the use to isPointInside based on a voxelized grid.
        void  enablePointCheck(float gridRes);
        void  make2D() {interpretAs2D = true;}
        void  make3D() {interpretAs2D = false;}

        short whereIsPoint(float* p);
        bool  isPointInside(float* p);
        bool  isPointInside(float* p, double& dist, int& faceInd, float* closestPoint);
        bool  isPointInside_basedOnWindingNumber(float* p); // This function might return true, also for open surfaces, if the winding_number > 0.5
        
        // Checks if a segment intersects the surface or not. 
        // <isSegBegInside,isSegEndInside,distFromSegBegToMesh,intersectingFaceIndex,intersectionIsInsideToOutside,boundaryTransitionDist>
        // distFromBegToMesh      ≠ NAN if the segment is intersecting the mesh. Then intersectingFaceIndex and intersectionIsInsideToOutside are valid.
        // boundaryTransitionDist ≠ NAN if the segment is transitioning through the boundary without intersection.
        std::tuple<bool,bool,double,int,bool,double> intersectSegment(const LineSegment* seg);

        // TODO: The following should be moved to surface_operators and they should generate new surfaces.
        // Because they modify vertices and/or faces of the surface
        void applyAffineTransform(float **affT);
        void convertFSLsurface2RASmm(std::string inp_template);
        void convertFreesurferSurface2RASmm(std::string inp_template);
        //--------------------

        int nv;          // Number of vertices
        int nf;          // Number of faces
        int ne;          // Number of unique edges
        OpenOrClosed  openOrClosed;
        ManifoldOrNot manifoldOrNot;
        bool interpretAs2D;

        std::string extension;
        float **vertices;
        int **faces;

        // Eigen variables
        Eigen::MatrixXd V;
        Eigen::MatrixXi F;

        // Igl variables
        igl::FastWindingNumberBVH fwn_bvh;
        igl::AABB<Eigen::MatrixXd,3> AABB_tree;        

        std::vector<SurfaceField> fields;

        float **centersOfFaces;
        float **normalsOfFaces;
        float *areasOfFaces;
        float **normalsOfVertices;
        float **triangleNormal;
        float **triangleEdge0;
        float **triangleEdge1;
        float **triangleEdge2;
        float  *gaussianCurvature;
        float  *meanCurvature;

        std::set<int> *neighboringVertices;
        std::set<int> *neighboringFaces;
        float area;
        float volume;

        std::vector<SurfaceField> findFields();
        static std::tuple<SurfaceField, std::vector<std::tuple<std::string, int, int, int, int>>> readFreesurferAnnot(std::string filePath, std::string LUTfname);

        SurfaceField readFreesurferLabel(std::string filePath);
        SurfaceField makeFieldFromFile(std::string filePath, std::string name, std::string owner, std::string dataType, int dimension, std::string LUTfname, bool isASCII);

        Surface& operator=(const Surface &surf);
        void copyMeshFrom(const Surface& obj);

        // Connected components
        std::vector<Surface>        comp;
        std::vector<OpenOrClosed>   compOpenOrClosed;
        std::vector<Surface>        compClosedAndOpen;
        std::vector<double>         compArea;
        std::vector<double>         compVolume;

        // Ordered boundaries
        void computeBoundaries();
        std::vector<std::vector<int>> boundaries; // For each boundary, there is a list of vertices
        std::vector<double>           boundaryLengths;
        std::vector<double>           boundaryAreas;

        // Vertex categorization
        void categorizeVertices();
        bool verticesCategorized;
        std::vector<int> singularVertices;
        std::vector<int> boundaryVertices;
        std::vector<int> overconnectedVertices;
        // std::vector<int> manifoldVertices;

        // Edge categorization
        void categorizeEdges();
        bool edgesCategorized;
        std::vector<std::pair<int, int>> boundaryEdges;
        std::vector<std::pair<int, int>> overconnectedEdges;
        // std::vector<std::pair<int, int>> manifoldEdges;

        // The following are used in isPointInside
        bool  enabledPointCheck;
        float pointCheckGridRes;
        std::vector<std::vector<std::vector<std::vector<int>>>> grid;
        NIBR::Image<int8_t> maskAndBoundary;

    private:
        std::string filePath;
        void copyFrom(const Surface& obj);

        bool readVTKMeshHeader();
        bool readGIIMeshHeader();
        bool readFreesurferMeshHeader();

        bool readVTKMesh();
        bool readGIIMesh();
        bool readFreesurferMesh();

        bool writeVTK(std::string _filePath);
        bool writeGII(std::string _filename);

        bool isClosedComp();       
        
    };

}
