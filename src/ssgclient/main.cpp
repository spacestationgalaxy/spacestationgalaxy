//
// Created by Nick on 1/4/2017.
//

// TODO: Check out OpenMPI hwloc api

// C++ STL
#include <iostream>
#include <stdexcept>
#include <functional>

// Platform-Specific
#ifdef _WIN64 // Windows
#   include <windows.h>
#else // Linux
#endif

// MinGW
//#include <pthread.h>

// 3rd Party
#ifdef HAS_BOOST
#   include <boost/array.hpp>
#   include <boost/container/vector.hpp>
#   include <boost/container/deque.hpp>
#   include <boost/thread.hpp>
#   include <boost/timer/timer.hpp>
#   include <boost/fiber/fiber.hpp>
#   include <boost/pool/pool.hpp>
#   include <boost/pool/object_pool.hpp>
#endif

#ifdef HAS_VULKAN
#   include <vulkan/vulkan.h>
#endif

#ifdef HAS_GLFW
#   define GLFW_INCLUDE_VULKAN
#   include <glfw/glfw3.h>
#endif

#ifdef HAS_GLM
#   define GLM_FORCE_RADIANS
#   define GLM_FORCE_DEPTH_ZERO_TO_ONE
#   include <glm/vec2.hpp>
#   include <glm/vec3.hpp>
#   include <glm/vec4.hpp>
#   include <glm/mat3x3.hpp>
#   include <glm/mat4x4.hpp>
#endif

#ifdef HAS_SSG_LIB
#   include "../ssg/plugin.h"
#endif

static boost::array<std::string, 5> gszTitle = {
        "==============================",
        "== SpaceStationGalaxy  v1.0 ==",
        "==============================",
        "====        CLIENT        ====",
        "=============================="
};

using boost::mutex;
using boost::condition_variable;
using boost::unique_lock;

static mutex mxIO;
static mutex mxThread;
static mutex mxThreadPreventDie;
static condition_variable cvForThreadsStart;

void boostExamples();
void vulkanExamples();

void fnThreadWorkLoop();

int main(int argc, char** argv)
{
    glfwInit();

    // Create application window
    int const winWidth = 1024;
    int const winHeight = 768;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Vulkan Window", nullptr, nullptr);

    VkApplicationInfo ai;
    memset(&ai, 0, sizeof(VkApplicationInfo));
    ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    ai.pNext = nullptr;
    ai.pApplicationName = "Hello Triangle";
    ai.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    ai.pEngineName = "No Engine";
    ai.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    ai.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfwExtCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtCount);

    VkInstanceCreateInfo ici;
    memset(&ici, 0, sizeof(VkInstanceCreateInfo));
    ici.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ici.pNext = nullptr;
    ici.flags = 0;
    ici.pApplicationInfo = &ai;
    ici.enabledExtensionCount = glfwExtCount;
    ici.ppEnabledExtensionNames = glfwExtensions;
    ici.enabledLayerCount = 0;
    ici.ppEnabledLayerNames = nullptr;

    VkInstance vki;
    memset(&vki, 0, sizeof(VkInstance));
    VkResult result = vkCreateInstance(&ici, nullptr, &vki);
    std::cout << result << std::endl;

    while (!glfwWindowShouldClose(window))
        glfwPollEvents();

    vkDestroyInstance(vki, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();

//    glm::mat4 matrix;
//    glm::vec4 vec;
//    glm::vec4 test = matrix * vec;

    //boostExamples();
    return EXIT_SUCCESS;
}

void boostExamples()
{
    using std::cout;
    using std::endl;

//    // Process Example
//    boost::process::ipstream pipe_stream;
//    boost::process::child c("gcc --version", boost::process::std_out > pipe_stream);
//
//    std::string line;
//    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty()) {
//        std::cerr << line << std::endl;
//    }
//    c.wait();

    // Timer Example
    using boost::timer::cpu_timer;
    using boost::timer::cpu_times;
    using boost::timer::nanosecond_type;

    cpu_timer timer;
    cpu_times time1(timer.elapsed());
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    cpu_times time2(timer.elapsed());
    double delta = (double(time2.wall - time1.wall)) / 1E9;
    std::cout << delta << std::endl;

    // Thread Example
    using boost::thread;

    {
        boost::unique_lock<boost::mutex> lk(mxIO);
        cout << "All Threads Starting" << endl;
    }

    // Create and initialize threads.
    boost::container::vector<boost::thread> vecThreads;
    {
        boost::unique_lock<boost::mutex> lkApp(mxThreadPreventDie);
        unsigned int const coreCount = boost::thread::hardware_concurrency();
        vecThreads.resize(coreCount);
        for (int i = 0; i < vecThreads.size(); ++i)
            vecThreads[i] = boost::thread(fnThreadWorkLoop);

        unsigned __int64 cpu;
        unsigned __int64 cpu_flag;
        // Pin each thread to a single core
        for (int i = 0; i < vecThreads.size(); ++i) {
            cpu = i % coreCount;
            cpu_flag = (1ULL << cpu);
#ifdef _WIN64 // Windows
            unsigned __int64 affMask = 0;
            affMask = cpu_flag;
            SetThreadAffinityMask(vecThreads[i].native_handle(), affMask);
            cout << "T: " << i << " A: " << affMask << endl;
#else // Linux
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(cpu, &cpuset);

            pthread_setaffinity_np(vecThreads[i].native_handle(), sizeof(cpu_set_t), &cpuset);
            cout << "T: " << i << " A: " << cpuset << endl;
#endif
        }

        // Feed initial tasks to threads.
        {
            boost::unique_lock<boost::mutex> lk(mxThread);
            cvForThreadsStart.notify_all();
        }

        // Check for finishers

        // If all threads are finished, sync and re-add jobs.

        // Perform EventLoop activities.
        // -- Feed additional tasks to thread queues until no more tasks exist OR queues are full.
        // -- Check for sync states for the next "frame"
        // -- Handle external IO/Window Functions/etc
        // -- If nothing else, work on tasks if addtl tasks exists.
        // -- Otherwise, spin lock until one of the above conditions happen.
        // -- On quit, signal to threads to prevent performing additional tasks.
    }

    // Join threads
    for (unsigned int i = 0; i < vecThreads.size(); ++i)
        vecThreads[i].join();

    {
        boost::unique_lock<boost::mutex> lk(mxIO);
        cout << "All Threads Finished" << endl;
    }
}

void vulkanExamples()
{
    using std::cout;
    using std::endl;

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    {
        boost::unique_lock<boost::mutex> lk(mxIO);
        cout << extensionCount << " extensions supported" << endl;

        for (int i = 0; i < gszTitle.size(); ++i)
            cout << gszTitle[i] << endl;
        cout << endl;
    }
}


void fnThreadWorkLoop()
{
    using std::cout;
    using std::endl;

    // Grab mutex lock for id printout
//    {
//        boost::unique_lock<boost::mutex> lk(mxThread);
//        cvForThreadsPrintoutIds.wait(lk);
//
//        {
//            boost::unique_lock<boost::mutex> lk(mxIO);
//            cout << "Thread ID: " << boost::this_thread::get_id() << endl;
//        }
//    }

    // Grab mutex lock and wait for start
    {
        boost::unique_lock<boost::mutex> lk(mxThread);
        cvForThreadsStart.wait(lk);
    }

    // Boost Pool creation
    using namespace glm;

    //boost::pool<> facVec2(sizeof(vec2));
    boost::object_pool<vec2> facVec2;
    vec2* v2test = facVec2.malloc();

    v2test->x = 5.0f;
    v2test->y = 10.0f;

    facVec2.free(v2test);

    while (true)
    {
        boost::mutex::scoped_lock lkDieCheck(mxThreadPreventDie, boost::try_to_lock);
        if (lkDieCheck)
            break;

        {
            boost::unique_lock<boost::mutex> lk(mxIO);
            cout << "Work..." << endl;
        }
    }

}