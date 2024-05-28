module;

import <glm/glm.hpp>;

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
    
    */
    export struct Instruction {
        alignas(4) uint32_t type;
        alignas(16) glm::vec4 valueA;
        alignas(16) glm::vec4 valueB;
    };
}