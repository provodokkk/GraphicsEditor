#include "Elements.h"

using namespace sf;
using namespace std;


void draw_dot(RenderWindow& window, float x, float y)
{
	CircleShape dot(4.f);
	dot.setFillColor(Color(0, 300, 0));
	dot.move(x - 2, y - 2);
	window.draw(dot);
}

void client_code()
{
	Component* tree = new MyGroup("Root");

	Component* branch_1 = new MyGroup;
	Component* branch_2 = new MyGroup;
	Component* branch_3 = new MyGroup;

	// branch_1
	Component* leaf = new Circle(20, 110, 110);
	Component* leaf_1 = new Circle(50, 30, 30);

	// branch_2
	Component* leaf_2 = new Circle(40, 175, 175);
	Component* leaf_4 = new Rectangle(40, 20, 260, 360);
	Component* leaf_6 = new Circle(40, 220, 50);

	// branch_3
	Component* leaf_3 = new Rectangle(40, 20, 60, 170);
	Component* leaf_5 = new Circle(40, 50, 275);

	// independent elements
	Component* leaf_7 = leaf_6->copy_elem();
	Component* leaf_8 = new Circle(30, 370, 300);
	
	
	// tree
	tree->add(branch_2);
	tree->add(branch_3);

	// independent elements
	tree->add(leaf_7);
	tree->add(leaf_8);
	
	// branch_1
	branch_1->add(leaf);
	branch_1->add(leaf_1);
	
	// branch_2
	branch_2->add(leaf_2);
	branch_2->add(leaf_4);
	branch_2->add(leaf_6);

	// branch_3
	branch_3->add(leaf_3);
	branch_3->add(leaf_5);

	// branch_4
	Component* branch_4 = branch_3->copy_elem();
	tree->add(branch_4);

	// relocating composite element branch_3 to branch_1
	branch_2->add(branch_1);
	branch_1->relocate_elem(branch_3);

	//branch_2->remove_elem(branch_1);

	branch_2->repositioning(150, 30);
	branch_4->repositioning(20, 20);
	leaf_7->repositioning(460, 60);
	leaf_8->repositioning(470, 140);

	tree->information_output();


	RenderWindow window(VideoMode(800, 500), "Graphics editor");
	Vector2i position;

	while (window.isOpen()) {
		window.clear(Color(70, 130, 170, 0));
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}

			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					position = Mouse::getPosition(window);

					cout << "\n\n-------------------------------------------------\n";
					cout << "x = " << position.x << endl << "y = " << position.y << endl;
					
					Component* component = nullptr;
					component = tree->search_by_coord(component, static_cast<float>(position.x), static_cast<float>(position.y));

					if (component) {
						component->information_output();
					}
					else {
						cout << "Free space!\n";
					}

					cout << "-------------------------------------------------\n";
					goto dot;
				}
			}
		}

		tree->drawing(window);
		dot: draw_dot(window, position.x, position.y);

		window.display();
	}

	tree->free_memory();
}

int main()
{
	client_code();
	cout << endl;
	return 0;
}