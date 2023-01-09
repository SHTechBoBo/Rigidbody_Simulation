#include "cloth.h"
#include "scene.h"
#include "ball.h"
#include "wall.h"
#include "bunny.h"
#include "object.h"
#include <random>

int main() {

  /// settings

  // window
  constexpr int window_width = 1920;
  constexpr int window_height = 1080;

  // cloth
  constexpr Float cloth_weight = Float(2);
  constexpr UVec2 mass_dim = { 40, 30 };
  constexpr Float dx_local = Float(0.1);
  constexpr Float stiffness = Float(15);
  constexpr Float damping_ratio = Float(0.0015);
  std::vector<IVec2> fixed_masses { { 0, -1 }, { -1, -1 } };

  //生成标识
  bool b = false;
  bool r = false;
  int id = 1;

  /// setup window
  GLFWwindow* window;
  {
    if (!glfwInit()) // initialize glfw library
      return -1;

    // setting glfw window hints and global configurations
    {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core mode
      // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // use debug context
    #ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif
    }

    // create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(window_width, window_height, "CS171 HW5: Cloth Simulation", NULL, NULL);
    if (!window) {
      glfwTerminate();
      return -1;
    }

    // make the window's context current
    glfwMakeContextCurrent(window);

    // load Opengl
    if (!gladLoadGL()) {
      glfwTerminate();
      return -1;
    }

    // setup call back functions
    glfwSetFramebufferSizeCallback(window, Input::CallBackResizeFlareBuffer);
  }

  /// main Loop
  {
    // shader
    Shader::Initialize();

    // scene
    Scene scene(45);
    scene.camera.transform.position = { 0, 1, -10 };
    scene.camera.transform.rotation = { 0, 0, 1, 0 };
    scene.light_position = { 0, 3, -10 };
    scene.light_color = Vec3(1, 1, 1) * Float(1.125);

    if (id == 0)
    {
        auto mesh_wall = std::make_shared<Wall>(Vec3(0, 0, 10), Vec3(10, 1, 10), Vec3(0, 0, 0));
        auto object_wall = scene.AddObject(mesh_wall,
            Shader::shader_phong,
            Transform(Vec3(0, 0, 0),
                Quat(1, 0, 0, 0),
                Vec3(1, 1, 1)));
        object_wall->color = { Float(0.75), one, 0.5f };
        object_wall->SetTag("Wall");

        auto mesh_wall1 = std::make_shared<Wall>(Vec3(5, -2, 10), Vec3(1, 10, 10), Vec3(0, 0, 0));
        auto object_wall1 = scene.AddObject(mesh_wall1,
            Shader::shader_phong,
            Transform(Vec3(0, 0, 0),
                Quat(1, 0, 0, 0),
                Vec3(1, 1, 1)));
        object_wall1->color = { Float(0.75), one, 0.5f };
        object_wall1->SetTag("Wall");
    }
    else if (id == 1)
    {
        auto mesh_wall = std::make_shared<Wall>(Vec3(0, -4, 2), Vec3(10, 1, 5), Vec3(0, 0, 0));
        auto object_wall = scene.AddObject(mesh_wall,
            Shader::shader_phong,
            Transform(Vec3(0, 0, 0),
                Quat(1, 0, 0, 0),
                Vec3(1, 1, 1)));
        object_wall->color = { Float(0.75), one, 0.5f };
        object_wall->SetTag("Wall");

        auto mesh_wall1 = std::make_shared<Wall>(Vec3(0, 0, 10), Vec3(10, 1, 10), Vec3(-60, 0, 0));
        auto object_wall1 = scene.AddObject(mesh_wall1,
            Shader::shader_phong,
            Transform(Vec3(0, 0, 0),
                Quat(1, 0, 0, 0),
                Vec3(1, 1, 1)));
        object_wall1->color = { Float(0.75), one, 0.5f };
        object_wall1->SetTag("Wall");
    }
    else if (id == 2)
    {
        auto mesh_wall = std::make_shared<Wall>(Vec3(0, -4, 2), Vec3(100, 1, 100), Vec3(0, 0, 0));
        auto object_wall = scene.AddObject(mesh_wall,
            Shader::shader_phong,
            Transform(Vec3(0, 0, 0),
                Quat(1, 0, 0, 0),
                Vec3(1, 1, 1)));
        object_wall->color = { Float(0.75), one, 0.5f };
        object_wall->SetTag("Wall");

    }


    // loop until the user closes the window
    Input::Start(window);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
      Input::Update();
      Time::Update();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /// terminate
      if (Input::GetKey(KeyCode::Escape))
        glfwSetWindowShouldClose(window, true);

      //按键添加球体
      if (!b && Input::GetKey(KeyCode::B))
       {
          auto mesh_ball = std::make_shared<Ball>();
          auto ball = scene.AddObject(mesh_ball,
                                    Shader::shader_phong,
                                    Transform(Vec3(float(rand() % 100)/100 - 0.5f, 4, float(rand() % 100) / 100 - 0.5f + 10),
                                        Quat(1, 0, 0, 0),
                                        Vec3(1, 1, 1)));
          ball->SetTag("Ball");
          ball->color = { float(rand() % 100) / 100, float(rand() % 100) / 100, float(rand() % 100) / 100 };
          b = true;
       }
      if (!Input::GetKey(KeyCode::B))
          b = false;
      
      //按键添加兔子
      if (!r && Input::GetKey(KeyCode::R))
      {
          auto mesh_bunny = std::make_shared<Bunny>();
          auto bunny = scene.AddObject(mesh_bunny,
              Shader::shader_phong,
              Transform(Vec3(2, 2, 0.3),
                  Quat(1, 0, 0, 0),
                  Vec3(1, 1, 1)));
          bunny->color = { Float(0.25), Float(0.8), zero};
          bunny->SetTag("Bunny");
          r = true;
      }
      if (!Input::GetKey(KeyCode::R))
          r = false;

      //Reset
      if (Input::GetKey(KeyCode::Q))
          scene.Reset();

      /// fixed update
      //物体更新位置
      for (unsigned i = 0; i < Time::fixed_update_times_this_frame; ++i) {
        if(Input::GetKey(KeyCode::Space)) { //! only when space is pressed
          scene.FixedUpdate();
        }
      }

      /// update
      //相机更新
      {
        scene.Update();
      }

      /// render
      //绘制所有obj
      {
        scene.RenderUpdate();
      }

      // swap front and back buffers
      glfwSwapBuffers(window);

      // poll for and process events
      glfwPollEvents();
    }
  }

  glfwTerminate();
  return 0;
}
