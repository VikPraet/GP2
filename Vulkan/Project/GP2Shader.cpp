#include "GP2Shader.h"
#include "vulkanbase/VulkanUtil.h"

void GP2Shader::Initialize(const VkDevice& vkDevice)
{
	m_ShaderStages.push_back(createVertexShaderInfo(vkDevice));
	m_ShaderStages.push_back(createFragmentShaderInfo(vkDevice));
}

void GP2Shader::DestroyShaderModules(const VkDevice& vkDevice)
{
	for(auto& stageInfo: m_ShaderStages)
	{
		vkDestroyShaderModule(vkDevice, stageInfo.module, nullptr);
	}
	m_ShaderStages.clear();
}

VkPipelineShaderStageCreateInfo GP2Shader::createFragmentShaderInfo(const VkDevice& vkDevice) {
	std::vector<char> fragShaderCode = readFile(m_FragmentShaderFile);
	VkShaderModule fragShaderModule = createShaderModule(vkDevice, fragShaderCode);

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	return fragShaderStageInfo;
}

VkPipelineShaderStageCreateInfo GP2Shader::createVertexShaderInfo(const VkDevice& vkDevice) {
	std::vector<char> vertShaderCode = readFile(m_VertexShaderFile);
	VkShaderModule vertShaderModule = createShaderModule(vkDevice, vertShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	return vertShaderStageInfo;
}

VkPipelineVertexInputStateCreateInfo GP2Shader::createVertexInputStateInfo()
{
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	m_BindingDescription = Vertex::getBindingDescription();
	m_AttributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_AttributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &m_BindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = m_AttributeDescriptions.data();
	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo GP2Shader::createInputAssemblyStateInfo()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	return inputAssembly;
}

VkShaderModule GP2Shader::createShaderModule(const VkDevice& vkDevice, const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}