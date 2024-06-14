module;

#include <glm/glm.hpp>;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

export module Engine4D.Structs;

namespace Engine4D
{
    /*

    Type Guidelines:

        Shapes:
        1 - HyperSphere     2 - Tesseract           3 - HyperPlane              4 - Tetrahedronal Extrusion
        5 - Spherinder      6 - Conical Extrusion   7 - Cylindrical Extrusion   8 - Toroidal Extrusion

        Operations:
        9 - Translation     10 - Rotation           11 - Scaling                12 - Inverse
        13 - Union          14 - Intersection       15 - Difference             16 - Hollow
        17 - Smooth Union   18 - Smooth Intersect   19 - Smooth Difference      20 - Smooth Hollow

        Other:
        21 - Material

    */

    export constexpr int MAX_INSTRUCTIONS = 200;

    export struct Instruction {
        uint8_t type;
        glm::mat4 valueA;
        glm::vec4 valueB;
    };

    export struct InstructionData {
        alignas(16) glm::mat4 valueA;
        alignas(4) float floatB;
        alignas(4) float floatC;
        alignas(4) float floatD;
        alignas(4) uint32_t valueE;
    };

    export InstructionData InstructionToData(Instruction instruction) {
        InstructionData data = InstructionData();
        data.valueA = instruction.valueA;
        data.floatB = instruction.valueB.x;
        data.floatC = instruction.valueB.y;
        data.floatD = instruction.valueB.z;
        int _valueE = (int32_t)(instruction.valueB.w * 10000);
        data.valueE = _valueE * 64 + instruction.type;
        return data;
    }

    export struct CameraInfo {
        float rotation;
        glm::vec4 cameraPosition;
    };

    export struct FrameInfo {
        int frameIndex;
        VkCommandBuffer commandBuffer;
        CameraInfo cameraInfo;
        VkDescriptorSet globalDescriptorSet;
        int instructionsCount;
    };
}