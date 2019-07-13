/* 
 * Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Precompiled.h"

namespace Framework
{
    //// TODO(jserrano): check for vulkan support
    ////bool platformSupportsVulkan;
    ////system_info_get_platform_bool("http://tizen.org/feature/vulkan.version.1_0", &platformSupportsVulkan);

    //// Gets the names of the extensions needed to communicate Vulkan with the current platform window system
    //void RetrieveSDLVulkanExtensions(Window* sdlWindow)
    //{
    //    uint32 enabledExtensionCount;
    //    // get count of required extensions
    //    if (!SDL_Vulkan_GetInstanceExtensions(window, &enabledExtensionCount, NULL))
    //        handle_error();

    //    static const char *const additionalExtensions[] =
    //    {
    //        VK_EXT_DEBUG_REPORT_EXTENSION_NAME, // example additional extension
    //    };
    //    size_t additionalExtensionsCount = sizeof(additionalExtensions) / sizeof(additionalExtensions[0]);
    //    size_t extensionCount = count + additionalExtensionsCount;
    //    const char **names = malloc(sizeof(const char *) * extensionCount);
    //    if (!names)
    //        handle_error();

    //    // get names of required extensions
    //    if (!SDL_Vulkan_GetInstanceExtensions(window, &count, names))
    //        handle_error();

    //    // copy additional extensions after required extensions
    //    for (size_t i = 0; i < additionalExtensionsCount; i++)
    //        names[i + count] = additionalExtensions[i];

    //    VkInstanceCreateInfo instanceCreateInfo = {};
    //    instanceCreateInfo.enabledExtensionCount = extensionCount;
    //    instanceCreateInfo.ppEnabledExtensionNames = names;
    //    // fill in rest of instanceCreateInfo

    //    VkInstance instance;
    //    // create the Vulkan instance
    //    VkResult result = vkCreateInstance(&instanceCreateInfo, NULL, &instance);
    //    free(names);

    //    uint32 enabledExtensionCount = 0;
    //    const char** enabledExtensionNames = nullptr;
    //    {
    //        const SDL_bool result = SDL_Vulkan_GetInstanceExtensions(sdlWindow, &enabledExtensionCount, enabledExtensionNames);
    //        ASSERT(result == SDL_TRUE);
    //    }
    //}

    //void CreateVulkanDriverInstance(const char* appName, const char* engineName)
    //{
    //    VkApplicationInfo appInfo  = {};
    //    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    //    appInfo.pApplicationName   = appName;
    //    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    //    appInfo.pEngineName        = engineName;
    //    appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    //    appInfo.apiVersion         = VK_API_VERSION_1_0;

    //    VkInstanceCreateInfo createInfo = {};
    //    createInfo.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    //    createInfo.pApplicationInfo     = &appInfo;

    //    
    //}

    //void Initialize(const std::string& appName, SDL_Window* sdlWindow)
    //{
    //    
    //    
    //    uint32 enabledExtensionCount = 0;
    //    const char** enabledExtensionNames = nullptr;
    //    {
    //        const SDL_bool result = SDL_Vulkan_GetInstanceExtensions(sdlWindow, &enabledExtensionCount, enabledExtensionNames);
    //        ASSERT(result == SDL_TRUE);
    //    }

    //    uint32 enabledLayerCount = 0;
    //    VkInstanceCreateInfo createInfo
    //    {
    //        /*.sType                   =*/ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    //        /*.pNext                   =*/ nullptr,
    //        /*.flags                   =*/ 0,
    //        /*.pApplicationInfo        =*/ &appInfo,
    //        /*.enabledLayerCount       =*/ enabledLayerCount,
    //        /*.ppEnabledLayerNames     =*/ (const char *const *)nullptr,
    //        /*.enabledExtensionCount   =*/ enabledExtensionCount,
    //        /*.ppEnabledExtensionNames =*/ (const char *const *)enabledExtensionNames,
    //    };

    //    VkInstance instance;
    //    {
    //        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    //        ASSERT(result == VK_SUCCESS);
    //    }

    //    uint32_t deviceCount = 0;
    //    {
    //        VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    //        ASSERT(result == VK_SUCCESS);
    //    }

    //    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    //    {
    //        VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());
    //        ASSERT(result == VK_SUCCESS);
    //    }
    //    VkPhysicalDevice gpu = physicalDevices[0];

    //    uint32 queueCount = 0;
    //    VkQueueFamilyProperties* queueProperties;
    //    {
    //        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueCount, queueProperties);
    //        ASSERT(queueCount > 0);
    //    }

    //    VkPhysicalDeviceFeatures features;
    //    vkGetPhysicalDeviceFeatures(gpu, &features);

    //    uint32 graphicsQueueNodeIndex = UINT32_MAX;
    //    for (uint32 i = 0; i < queueCount; i++)
    //    {
    //        if ((queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
    //        {
    //            // TODO(jserrano): check why this shit
    //            if (graphicsQueueNodeIndex == UINT32_MAX)
    //                graphicsQueueNodeIndex = i;
    //        }
    //    }

    //    //****

    //    float queuePriorities[] { 0.0 };
    //    const VkDeviceQueueCreateInfo queue
    //    {
    //        /*.sType            =*/ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    //        /*.pNext            =*/ nullptr,
    //        /*.flags            =*/ 0,
    //        /*.queueFamilyIndex =*/ graphicsQueueNodeIndex,
    //        /*.queueCount       =*/ 1,
    //        /*.pQueuePriorities =*/ queuePriorities
    //    };

    //    VkDeviceCreateInfo deviceCreateInfo
    //    {
    //        /*.sType                   =*/ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    //        /*.pNext                   =*/ nullptr,
    //        /*.flags                   =*/ 0,
    //        /*.queueCreateInfoCount    =*/ 1,
    //        /*.pQueueCreateInfos       =*/ &queue,
    //        /*.enabledLayerCount       =*/ enabledLayerCount,
    //        /*.ppEnabledLayerNames     =*/ (const char *const *)/*(demo->validate) ? demo->device_validation_layers :*/ nullptr,
    //        /*.enabledExtensionCount   =*/ enabledExtensionCount,
    //        /*.ppEnabledExtensionNames =*/ (const char *const *)enabledExtensionNames,
    //        /*.pEnabledFeatures        =*/ nullptr
    //    };

    //    VkDevice device;
    //    {
    //        VkResult result = vkCreateDevice(gpu, &deviceCreateInfo, nullptr, &device);
    //        ASSERT(result == VK_SUCCESS);
    //    }

    //    VkSurfaceKHR surface;
    //    {
    //        const SDL_bool result = SDL_Vulkan_CreateSurface(sdlWindow, (SDL_vulkanInstance)instance, (SDL_vulkanSurface*)&surface);
    //        ASSERT(result == SDL_TRUE);
    //    }
    //}
}