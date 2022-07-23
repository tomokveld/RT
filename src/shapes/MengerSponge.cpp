#include "MengerSponge.hpp"

std::shared_ptr<CSG> MengerSponge(int m) {
    std::shared_ptr<Group> c1 = MakeCubes(-1.5, -1.5, 1.5, 1.5, 0, m);
    std::shared_ptr<Group> c2 = MakeCubes(-1.5, -1.5, 1.5, 1.5, 0, m);
    std::shared_ptr<Group> c3 = MakeCubes(-1.5, -1.5, 1.5, 1.5, 0, m);

    c2->set_transform(Transform::rotation_x(M_PI / 2.0f));
    c3->set_transform(Transform::rotation_y(M_PI / 2.0f));

    std::shared_ptr<CSG> u1 = std::make_shared<CSG>(std::make_shared<csgUnionOperator>(), c1, c2);
    std::shared_ptr<CSG> u2 = std::make_shared<CSG>(std::make_shared<csgUnionOperator>(), u1, c3);

    std::shared_ptr<Cube> cube = std::make_shared<Cube>();
    cube->set_transform(Transform::scaling(1.5, 1.5, 1.5));

    std::shared_ptr<CSG> out = std::make_shared<CSG>(std::make_shared<csgDifferenceOperator>(), cube, u2);
    return out;
}

std::shared_ptr<Group> MakeCubes(double x1, double y1, double x2, double y2, int n, int max) {
    std::shared_ptr<Group> g = std::make_shared<Group>();

    const double deltaX = (x2 - x1) / 3.0;
    const double deltaY = (y2 - y1) / 3.0;

    const double sx = deltaX / 2.0;
    const double sy = deltaY / 2.0;

    std::shared_ptr<Cube> cube = std::make_shared<Cube>();
    cube->mod_material().set_color(Color(1, 0, 0));
    cube->mod_material().set_shadow(false);
    const double tx = x1 + (x2 - x1) / 2.0;
    const double ty = y1 + (y2 - y1) / 2.0;
    cube->set_transform(Transform::translation(tx, ty, 0) * Transform::scaling(sx, sy, 2.1));

    g->add_child(cube);
    if (n < max) {
        // left col
        std::shared_ptr<Group> g1 = MakeCubes(x1 + 0*deltaX, y1+0*deltaY, x1 + 1*deltaX, y1 + 1*deltaY, n + 1, max);
        std::shared_ptr<Group> g2 = MakeCubes(x1 + 0*deltaX, y1+1*deltaY, x1 + 1*deltaX, y1 + 2*deltaY, n + 1, max);
        std::shared_ptr<Group> g3 = MakeCubes(x1 + 0*deltaX, y1+2*deltaY, x1 + 1*deltaX, y1 + 3*deltaY, n + 1, max);

        // center col
        std::shared_ptr<Group> g4 = MakeCubes(x1 + 1*deltaX, y1+0*deltaY, x1 + 2*deltaX, y1 + 1*deltaY, n + 1, max);
//        std::shared_ptr<Group> g5 = MakeCubes(x1 + 1*deltaX, y1+1*deltaY, x1 + 2*deltaX, y1 + 2*deltaY, n + 1, max);
        std::shared_ptr<Group> g6 = MakeCubes(x1 + 1*deltaX, y1+2*deltaY, x1 + 2*deltaX, y1 + 3*deltaY, n + 1, max);

       // right col
        std::shared_ptr<Group> g7 = MakeCubes(x1 + 2*deltaX, y1+0*deltaY, x1 + 3*deltaX, y1 + 1*deltaY, n + 1, max);
        std::shared_ptr<Group> g8 = MakeCubes(x1 + 2*deltaX, y1+1*deltaY, x1 + 3*deltaX, y1 + 2*deltaY, n + 1, max);
        std::shared_ptr<Group> g9 = MakeCubes(x1 + 2*deltaX, y1+2*deltaY, x1 + 3*deltaX, y1 + 3*deltaY, n + 1, max);
        g->add_children(g1, g2, g3, g4, g6, g7, g8, g9);
    }
    return g;
}
