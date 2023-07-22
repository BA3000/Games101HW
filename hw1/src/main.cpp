#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

inline double Deg2Rad(double degree) {
    return degree * MY_PI / 180.0f;
}

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0],
                0, 1, 0, -eye_pos[1],
                0, 0, 1, -eye_pos[2],
                0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model;

    // Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    double rad = Deg2Rad(rotation_angle);
    model << cos(rad), -sin(rad), 0, 0,
            sin(rad), cos(rad), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f persp, ortho;
    auto fov_rad = Deg2Rad(eye_fov);
    auto half_fov_rad = Deg2Rad(eye_fov / 2.0f);
    auto n = zNear, f = zFar;
    // perspective matrix
    persp << n, 0, 0, 0,
            0, n, 0, 0,
            0, 0, n + f, -n * f,
            0, 0, 1, 0;

    double w, h, z;
    h = - abs(zNear) * tan(half_fov_rad) * 2;
    w = h * aspect_ratio;
    z = zFar - zNear;
    ortho << 2 / w, 0, 0, 0,
            0, 2 / h, 0, 0,
            0, 0, 2 / z, -(zFar + zNear) / 2,
            0, 0, 0, 1;

    return ortho * persp;
}

// rotate around arbitrary axis
Eigen::Matrix4f get_rotation(Vector3f axis, float angle) {
    // TODO: implement this
    auto rot = Eigen::Matrix4f::Identity();
    return rot;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
