// The code mainly refers to https://github.com/Tencent/ncnn


#include <stdio.h>
#include <algorithm>
#include <vector>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#if CV_VERSION_MAJOR >= 4
#include <opencv2/opencv.hpp>
#define CV_LOAD_IMAGE_COLOR cv::IMREAD_COLOR
#endif  // CV_VERSION_MAJOR >= 4

#include "net.h"
#include "gpu.h"

struct KeyPoint
{
    cv::Point2f p;
    float prob;
};

static int detect_posenet(const cv::Mat& bgr, std::vector<KeyPoint>& keypoints)
{
    ncnn::Net posenet;

#if NCNN_VULKAN
    posenet.opt.use_vulkan_compute = true;
#endif // NCNN_VULKAN

    
    posenet.load_param("hg_pose.param");
    posenet.load_model("hg_pose.bin");

    int w = bgr.cols;
    int h = bgr.rows;

    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR2RGB, w, h, 256, 256);

    const float mean_vals[3] = {0.4404f*255.f, 0.4440f*255.f, 0.4327f*255.f};
    const float norm_vals[3] = {1/255.f, 1/255.f, 1/255.f};
    in.substract_mean_normalize(mean_vals, norm_vals);

    ncnn::Extractor ex = posenet.create_extractor();

    clock_t start,end;
    start = clock();

    ex.input("0", in);
    ncnn::Mat out;
    ex.extract("940", out);
    
    end = clock();
    printf("per image time=%f s\n",(double)(end-start)/CLOCKS_PER_SEC);

    // resolve point from heatmap
    
    keypoints.clear();
    for (int p = 0; p < out.c; p++)
    {
        const ncnn::Mat m = out.channel(p);

        float max_prob = 0.f;
        int max_x = 0;
        int max_y = 0;
        for (int y = 0; y < out.h; y++)
        {
            const float* ptr = m.row(y);
            for (int x = 0; x < out.w; x++)
            {
                float prob = ptr[x];
                if (prob > max_prob)
                {
                    max_prob = prob;
                    max_x = x;
                    max_y = y;
                }
            }
        }

        KeyPoint keypoint;
        keypoint.p = cv::Point2f(max_x * w / (float)out.w, max_y * h / (float)out.h);
        keypoint.prob = max_prob;

        keypoints.push_back(keypoint);
    }
    return 0;
}

static void draw_pose(const cv::Mat& bgr, const std::vector<KeyPoint>& keypoints)
{
    cv::Mat image = bgr.clone();

    // draw bone
    static const int joint_pairs[16][2] = {
        {0, 1}, {1, 2}, {3, 4}, {4, 5},
        {2, 6}, {3, 6}, {8, 9}, {10, 11}, {11, 12},
        {13, 14}, {14, 15}, {7, 12},
        {7, 13}, {6, 8}};
    
    for (int i = 0; i < 14; i++)
    {
        const KeyPoint& p1 = keypoints[ joint_pairs[i][0] ];
        const KeyPoint& p2 = keypoints[ joint_pairs[i][1] ];

        if (p1.prob < 0.2f || p2.prob < 0.2f)
            continue;

        cv::line(image, p1.p, p2.p, cv::Scalar(255, 255, 255), 3);
    }

    // draw joint
    for (size_t i = 0; i < keypoints.size(); i++)
    {
        const KeyPoint& keypoint = keypoints[i];

        //fprintf(stderr, "%.2f %.2f = %.5f\n", keypoint.p.x, keypoint.p.y, keypoint.prob);

        if (keypoint.prob < 0.2f)
            continue;

        cv::circle(image, keypoint.p, 5, cv::Scalar(0, 0, 255), -1);
    }

    cv::imshow("image", image);
    cv::waitKey(0);
}

int main(int argc, char** argv)
{
    const char* imagepath = argv[1];

    cv::Mat m = cv::imread(imagepath, CV_LOAD_IMAGE_COLOR);
    if (m.empty())
    {
        fprintf(stderr, "cv::imread %s failed\n", imagepath);
        return -1;
    }

#if NCNN_VULKAN
    ncnn::create_gpu_instance();
#endif // NCNN_VULKAN

    std::vector<KeyPoint> keypoints;
    detect_posenet(m, keypoints);

    draw_pose(m, keypoints);

#if NCNN_VULKAN
    ncnn::destroy_gpu_instance();
#endif // NCNN_VULKAN

    return 0;
}
