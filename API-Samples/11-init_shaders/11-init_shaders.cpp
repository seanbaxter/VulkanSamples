/*
 * Vulkan Samples
 *
 * Copyright (C) 2015-2020 Valve Corporation
 * Copyright (C) 2015-2020 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
VULKAN_SAMPLE_SHORT_DESCRIPTION
Initialize Vertex and Fragment Shaders
*/

/* This is part of the draw cube progression */

#include <util_init.hpp>
#include <assert.h>
#include <string.h>
#include <cstdlib>

// Vertex stage.
[[using spirv: uniform, binding(0)]] mat4 mvp;
[[using spirv: in, location(0)]]     vec4 vs_inPos;
[[using spirv: in, location(1)]]     vec2 vs_inTexCoord;
[[using spirv: out, location(0)]]    vec2 vs_outTexCoord;

[[spirv::vert]]
void vert_shader() {
    vs_outTexCoord = vs_inTexCoord;
    glvert_Output.Position = mvp * vs_inPos;
}

// Fragment stage.
[[using spirv: uniform, binding(1)]] sampler2D tex;
[[using spirv: in, location(0)]]     vec2 fs_inTexCoord;
[[using spirv: out, location(0)]]    vec4 fs_outColor;

[[spirv::frag]]
void frag_shader() {
    fs_outColor = textureLod(tex, fs_inTexCoord, 0);
}

int sample_main(int argc, char *argv[]) {
    VkResult U_ASSERT_ONLY res;
    struct sample_info info = {};
    char sample_title[] = "Initialize Shaders Sample";

    init_global_layer_properties(info);
    init_instance(info, sample_title);
    init_enumerate_device(info);
    init_queue_family_index(info);
    init_device(info);

    // All shaders in the same translation unit are stored in the same module.
    VkShaderModuleCreateInfo moduleCreateInfo;
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = __spirv_size;
    moduleCreateInfo.pCode = (const uint32_t*)__spirv_data;

    VkShaderModule module;
    res = vkCreateShaderModule(info.device, &moduleCreateInfo, NULL, &module);
    assert(res == VK_SUCCESS);

    // Associate each shader stage with the same module.
    info.shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.shaderStages[0].pNext = NULL;
    info.shaderStages[0].pSpecializationInfo = NULL;
    info.shaderStages[0].flags = 0;
    info.shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    info.shaderStages[0].pName = @spirv(vert_shader);
    info.shaderStages[0].module = module;

    info.shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.shaderStages[1].pNext = NULL;
    info.shaderStages[1].pSpecializationInfo = NULL;
    info.shaderStages[1].flags = 0;
    info.shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    info.shaderStages[1].pName = @spirv(frag_shader);
    info.shaderStages[1].module = module;

    vkDestroyShaderModule(info.device, module, NULL);
    destroy_device(info);
    destroy_instance(info);
    return 0;
}
