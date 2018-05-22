#include "getRTAlgo.h"
#include "5point.h"
//#include "../5point.h"


using namespace cv;
using namespace std;

void _pixel2cam ( const double px, const double py, const Mat& K, double& cx, double& cy )
{
    cx = (px - K.at<double> ( 0, 2 )) / K.at<double> ( 0, 0 );
    cy = (py - K.at<double> ( 1, 2 )) / K.at<double> ( 1, 1 );
}

bool _calculateRT_5points_with_ratio_many_results( const vector<Point2f>& vpts1, const vector<Point2f>& vpts2, const Mat& K, vector<Mat>& R_vec, vector<Mat>& t_vec, vector<double>& inliers_ratio_vec, int ptsLimit, bool withDebug )
{
    int npts = static_cast<int>(vpts1.size());
    if (npts < 5) return false;
    int chosenNum = min(npts, ptsLimit);


    cout << "In calculateRT_5points, num of points: " << npts << ", chosenNum:" << chosenNum << endl;

    //pixel2cam
    vector<double> _pts1_cam, _pts2_cam;
    _pts1_cam.resize(chosenNum * 2);
    _pts2_cam.resize(chosenNum * 2);

    for (int i = 0; i < chosenNum; i++) {
        _pixel2cam (vpts1[i].x, vpts1[i].y, K, _pts1_cam[i * 2], _pts1_cam[i * 2 + 1]);
        _pixel2cam (vpts2[i].x, vpts2[i].y, K, _pts2_cam[i * 2], _pts2_cam[i * 2 + 1]);
    }


    vector<Mat> E; // essential matrix
    vector<Mat> P;
    vector<int> inliers;

    bool ret = Solve5PointEssential(_pts1_cam.data(), _pts2_cam.data(), chosenNum, E, P, inliers); // 从4个解得到1个最优解；P：映射矩阵 [R|t]
    if ( ret == false ) {
        cout << "Could not find a valid essential matrix" << endl;
        return false;
    }

    cout << "============== Solve5PointEssential START =============" << endl;
    printf("Solve5PointEssential() found %llu solutions:\n", E.size());

    for (size_t i = 0; i < E.size(); i++) {
        if (determinant(P[i](Range(0, 3), Range(0, 3))) < 0) P[i] = -P[i];

        Mat R = P[i].colRange(0, 3);
        Mat t = P[i].colRange(3, 4);
        double inliers_ratio = static_cast<double>(inliers[i]) / chosenNum;

        R_vec.push_back(R);
        t_vec.push_back(t);
        inliers_ratio_vec.push_back(inliers_ratio);
    }
    cout << "============== Solve5PointEssential  DONE =============" << endl;

    return true;
}


bool _calculateRT_5points_with_ratio ( const vector<Point2f>& vpts1, const vector<Point2f>& vpts2, const Mat& K, Mat& R, Mat& t, double& inliers_ratio, int ptsLimit, bool withDebug )
{
    vector<Mat> R_vec, t_vec;
    vector<double> inliers_ratio_vec;
    bool ret = _calculateRT_5points_with_ratio_many_results ( vpts1, vpts2, K, R_vec, t_vec, inliers_ratio_vec, vpts1.size (), true );

    if ( ret == false) {
        cout << "Could not find a valid essential matrix" << endl;
        return false;
    }

    auto max_value = std::max_element ( inliers_ratio_vec.begin (), inliers_ratio_vec.end () );
    int best_index = std::distance ( inliers_ratio_vec.begin (), max_value );
    std::cout << "max element at: " << best_index << '\n';

    R = R_vec[best_index];
    t = t_vec[best_index];

    return true;
}


bool calculateRT_5points( const vector<Point2f>& vpts1, const vector<Point2f>& vpts2, const Mat& K, Mat& R, Mat& t, int ptsLimit, bool withDebug )
{
    double inliers_ratio = 0.0;
    return _calculateRT_5points_with_ratio(vpts1, vpts2, K, R, t, inliers_ratio, ptsLimit, withDebug);
}