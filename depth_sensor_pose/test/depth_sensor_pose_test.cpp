#include <depth_sensor_pose/depth_sensor_pose.h>

#include <gtest/gtest.h>

class DepthSensorPoseTestable : public depth_sensor_pose::DepthSensorPose {
};

class DepthSensorPoseTest : public ::testing::Test {
 public:
  sensor_msgs::msg::Image::SharedPtr depth_msg;
  sensor_msgs::msg::CameraInfo::SharedPtr info_msg;
  DepthSensorPoseTestable estimator;

  unsigned img_height { 480 };
  unsigned img_width { 640 };
  unsigned scan_height { 420 };

  DepthSensorPoseTest() {
    setDefaultInfoMsg();

    // Configuration
    estimator.setDepthImgStepCol(8);
    estimator.setDepthImgStepRow(8);
    estimator.setGroundMaxPoints(2000);
    estimator.setRansacMaxIter(1000);
  }

  void setDefaultInfoMsg() {
    info_msg.reset(new sensor_msgs::msg::CameraInfo);
    info_msg->header.frame_id = "depth_frame";
    info_msg->height = img_height;
    info_msg->width = img_width;
    info_msg->distortion_model = "plumb_bob";
    info_msg->d.resize(5);
    info_msg->k[0] = 570;
    info_msg->k[2] = 314;
    info_msg->k[4] = 570;
    info_msg->k[5] = 239;
    info_msg->k[8] = 1.0;
    info_msg->r[0] = 1.0;
    info_msg->r[4] = 1.0;
    info_msg->r[8] = 1.0;
    info_msg->p[0] = 570;
    info_msg->p[2] = 314;
    info_msg->p[5] = 570;
    info_msg->p[6] = 235;
    info_msg->p[10] = 1.0;
  }

  template<typename T>
  void setDefaultDepthMsg(T value) {
    depth_msg.reset(new sensor_msgs::msg::Image);
    depth_msg->header.frame_id = "depth_frame";
    depth_msg->height = img_height;
    depth_msg->width = img_width;
    depth_msg->is_bigendian = false;
    depth_msg->step = depth_msg->width * sizeof(T);

    if (typeid(T) == typeid(uint16_t)) {
      depth_msg->encoding = sensor_msgs::image_encodings::TYPE_16UC1;
    }
    else if (typeid(T) == typeid(float)) {
      depth_msg->encoding = sensor_msgs::image_encodings::TYPE_32FC1;
    }

    depth_msg->data.resize(depth_msg->width * depth_msg->height * sizeof(T));
    T* depth_row = reinterpret_cast<T*>(&depth_msg->data[0]);
    for (size_t i = 0; i < depth_msg->width * depth_msg->height; ++i) {
        depth_row[i] = value;
    }
  }
};

TEST_F(DepthSensorPoseTest, encodingSupport)
{
  setDefaultDepthMsg<uint16_t>(1);
  estimator.estimateParams(depth_msg, info_msg);

  setDefaultDepthMsg<float>(1);
  estimator.estimateParams(depth_msg, info_msg);
}

// TEST_F(DepthSensorPoseTest, unsupportedEncoding)
// {
//   setDefaultDepthMsg<uint16_t>(1);
//   depth_msg->encoding = sensor_msgs::image_encodings::MONO16;
//   EXPECT_ANY_THROW(estimator.estimateParams(depth_msg, info_msg));
// }

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
