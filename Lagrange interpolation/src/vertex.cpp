#include "../include/vertex.h"

Vertex_c vertex;
std::vector<std::vector<Vertex_c>> vertices;

void Vertex_c::CreateVertices()
{
    std::vector<Vertex_c> square_2d_red = {
        Vertex_c{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {}, {}},
        Vertex_c{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {}, {}},
        Vertex_c{{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {}, {}},
        Vertex_c{{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {}, {}},
        Vertex_c{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {}, {}},
        Vertex_c{{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {}, {}}
    };
    vertices.push_back(square_2d_red); // 0
    
    std::vector<Vertex_c> square_2d_blue = {
        Vertex_c{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.8f}, {}, {}},
        Vertex_c{{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.8f}, {}, {}},
        Vertex_c{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.8f}, {}, {}},
        Vertex_c{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.8f}, {}, {}},
        Vertex_c{{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.8f}, {}, {}},
        Vertex_c{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.8f}, {}, {}}
    };
    vertices.push_back(square_2d_blue); // 1

    std::vector<Vertex_c> cube = {
        // front
        Vertex_c{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        Vertex_c{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        Vertex_c{{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        Vertex_c{{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        Vertex_c{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        Vertex_c{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        // back
        Vertex_c{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, 
        Vertex_c{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        Vertex_c{{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, 
        Vertex_c{{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, 
        Vertex_c{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, 
        Vertex_c{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, 
        // left
        Vertex_c{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}, 
        Vertex_c{{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        Vertex_c{{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}, 
        Vertex_c{{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}}, 
        Vertex_c{{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}}, 
        Vertex_c{{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}, 
        // right
        Vertex_c{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, 
        Vertex_c{{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        Vertex_c{{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, 
        Vertex_c{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, 
        Vertex_c{{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, 
        Vertex_c{{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, 
        // up
        Vertex_c{{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, 
        Vertex_c{{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        Vertex_c{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, 
        Vertex_c{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}, 
        Vertex_c{{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}, 
        Vertex_c{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, 
        // down
        Vertex_c{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, 
        Vertex_c{{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        Vertex_c{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, 
        Vertex_c{{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}}, 
        Vertex_c{{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}}, 
        Vertex_c{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, 
    };
    vertices.push_back(cube); // 2

    std::vector<Vertex_c> axies = {
        // red x-axies
        Vertex_c{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {}, {}},
        Vertex_c{{300.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {}, {}},
        // green y-axies
        Vertex_c{{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {}, {}},
        Vertex_c{{0.0f, 300.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {}, {}},
        // blue z-axies
        Vertex_c{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {}, {}},
        Vertex_c{{0.0f, 0.0f, 300.0f}, {0.0f, 0.0f, 1.0f}, {}, {}},
    };
    vertices.push_back(axies); // 3
}

void Vertex_c::DestroyVertices()
{
    vertices.clear();
}