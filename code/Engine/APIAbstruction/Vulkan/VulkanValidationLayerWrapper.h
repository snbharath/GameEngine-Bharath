#pragma once

#if defined(VULKAN)

#include <vector>

using namespace std;
namespace GE
{
	class VulkanValidationLayerWrapper
	{
	private:
		static VulkanValidationLayerWrapper* s_pInstance;

		VulkanValidationLayerWrapper();
		~VulkanValidationLayerWrapper() {};

		const vector<const char*> m_cValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const bool m_cEnableValidationLayers;
	public:
		static VulkanValidationLayerWrapper* GetInstance();

		static bool DeleteInstance();

		bool CheckValidationLayerSupport();

		inline bool IsEnableValidationLayer() const;

		void PrintAvailableValidationLayer();

		const vector<const char*> GetValidationLayers() const;
	};

	// inline functions
	inline bool VulkanValidationLayerWrapper::IsEnableValidationLayer() const
	{
		return m_cEnableValidationLayers;
	}
}

#endif