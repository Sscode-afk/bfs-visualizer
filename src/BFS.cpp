#include <SFML/Graphics.hpp>
#include <queue>

struct Node{
    
    float x;
    float y;
    std::vector<int> connected;
    sf::CircleShape shape;
};
struct Point {
    sf::Vector2f pos;
    sf::Vector2f target;
    int target_node;
};
class BFS{
    public:
        sf::RenderWindow window;
        sf::RenderTexture canvas;

        std::vector<Node> data = {
            {600.0f, 120.0f, {1, 2, 3}},    //  0  root
            {380.0f, 270.0f, {0, 4, 5}},    //  1
            {680.0f, 270.0f, {0, 6}},       //  2
            {980.0f, 270.0f, {0}},          //  3  leaf
            {180.0f, 440.0f, {1, 7, 8}},    //  4
            {480.0f, 440.0f, {1, 9}},       //  5
            {780.0f, 440.0f, {2, 10, 11}},  //  6
            {100.0f, 610.0f, {4, 12}},      //  7
            {260.0f, 610.0f, {4}},          //  8  leaf
            {480.0f, 610.0f, {5}},          //  9  leaf
            {720.0f, 610.0f, {6}},          // 10  leaf
            {900.0f, 610.0f, {6}},          // 11  leaf
            {100.0f, 780.0f, {7}},          // 12  leaf
        };
        std::vector<Point> points = {};
        bool start = false;
        bool check_next_node = true;
        sf::Clock clock;

        std::vector<bool> visited;
        std::queue<int> nqueue;

        float accumulator = 0;
        float timestep = 1.f/240.f;
        Node current;

        BFS() : window(sf::VideoMode({1200,900}),"Title"),canvas({1200,900}), visited(data.size(),false) {
            nqueue.push(0);
            visited[0] = true;
            for (auto& node:data) {
                node.shape.setRadius(10);
                node.shape.setFillColor(sf::Color::Red);
                sf::FloatRect bounds = node.shape.getLocalBounds();
                node.shape.setOrigin({bounds.position.x+bounds.size.x/2.f,bounds.position.y+bounds.size.y/2.f});
                node.shape.setPosition({node.x,node.y});
                
            }
            canvas.clear(sf::Color::Transparent);
        }

        void RUN() {
            while (window.isOpen()) {
                while (auto event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.close();
                    } else if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        if (key->scancode==sf::Keyboard::Scancode::Space) {
                            start = true;
                        }
                    }
                }
                window.clear(sf::Color::Black);
                float dt = clock.getElapsedTime().asSeconds();
                clock.restart();
                if (dt>=0.05) dt = 0.05;
                UPDATE(dt);
                sf::Sprite lines(canvas.getTexture());
                window.draw(lines);
                for (auto& node:data) {
                    window.draw(node.shape);
                }
                window.display();
            }
        }
        void UPDATE(float dt) {
            if (start) {
                update_node();
                
                accumulator+=dt;
                while (accumulator>=timestep && !points.empty()) {
                    for (auto it = points.begin();it!=points.end();) {
                        float dx = it->target.x-it->pos.x;
                        float dy = it->target.y-it->pos.y;
                        if (dx*dx+dy*dy <= 3*3) {
                            it->pos.x = it->target.x;
                            it->pos.y = it->target.y;
                        }
                        if (it->pos.x==it->target.x && it->pos.y==it->target.y) {
                            it = points.erase(it);
                        } else {
                            float prev_x = it->pos.x;
                            float prev_y = it->pos.y;
                            float dx = ((it->target.x-it->pos.x)/0.02) * timestep;
                            float dy = ((it->target.y-it->pos.y)/0.02) * timestep;
                            it->pos = {it->pos.x+dx,it->pos.y+dy};

                            sf::Vertex line[] = {
                                sf::Vertex{{prev_x,prev_y}},
                                sf::Vertex{{it->pos.x,it->pos.y}}
                            };
                            canvas.draw(line,2,sf::PrimitiveType::Lines);
                            canvas.display();
                            it++;
                        }
                    }
                    accumulator-=timestep;
                }
                if (points.empty()) {
                    check_next_node = true;
                }

            }
        }
        void update_node() {
            if (nqueue.empty()) {
                start = false;
                return;
            } else if (check_next_node){
                int first = std::move(nqueue.front());
                current = data[first];
                nqueue.pop();
                for (auto& node:current.connected) {
                    if (!visited[node]) {
                        nqueue.push(node);
                        visited[node] = true;
                        points.push_back({{current.x,current.y},{data[node].x,data[node].y},node});
                    }
                }
                check_next_node = false;
            }
        }
};

int main(){
    BFS instance;
    instance.RUN();
    return 0;
}