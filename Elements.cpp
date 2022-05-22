#include "Elements.h"


// class Component - basic

void Component::repositioning(float x, float y)
{
	float difference_x_pos = x_position - x;
	float difference_y_pos = y_position - y;
	relocating(difference_x_pos, difference_y_pos);
}

int Component::nesting_cnt()
{
	Component* _parent = parent;
	int _nesting = -1;
	while (_parent != nullptr)
	{
		++_nesting;
		_parent = _parent->get_parent();
	}
	return _nesting;
}

void Component::nesting_output()
{
	if (is_composite() || parent->get_parent() == nullptr) { cout << endl; }
	for (int i = 0; i < nesting_cnt(); ++i)
	{
		cout << "+";
	}
}

void Component::relocate_elem(Component* component)
{
	component->get_parent()->remove(component);
	add(component);
}

void Component::invalidate_element(Component* component)
{
	component->name = "NoName";
	component->x_position = component->y_position = 0;
	component->height = component->width = 0;
	component->set_parent(nullptr);
	component->invalidate_additional_parameters(component);
}

void Component::general_copy(Component* component) const
{
	component->name = name;
	component->x_position = x_position;
	component->y_position = y_position;
	component->height = height;
	component->width = width;
}


// class MyShape : public Component

void MyShape::relocating(float x, float y)
{
	x_position -= x;
	y_position -= y;
}

void MyShape::remove(Component* component)
{
	component->get_parent()->remove(component);
	component->set_parent(nullptr);
}

void MyShape::remove_elem(Component* component)
{
	invalidate_element(component);
	remove(component);
}

void MyShape::free_memory()
{
	cout << "\nClearing the memory of the " << name;
	delete this;
}


// class Circle : public MyShape

void Circle::information_output()
{
	if (outputting_flag == false)
	{
		outputting_flag = true;
		nesting_output();
		cout << name << " (" << x_position << ", " << y_position << ") R = " << radius << endl;
	}
}

Component* Circle::search_by_coord(Component* max_nesting_component, float x, float y)
{
	float x_center_circle = x_position + radius;
	float y_center_circle = y_position + radius;
	if (pow(radius, 2) >= pow((x - x_center_circle), 2) + pow((y - y_center_circle), 2))
	{
		return this;
	}
	return max_nesting_component;
}

void Circle::copy_additional_parameters(Component* component)
{
	component->set_radius(radius);
}

Component* Circle::copy_elem()
{
	Component* component = new Circle;
	general_copy(component);
	copy_additional_parameters(component);
	component->set_parent(get_parent());
	return component;
}

void Circle::drawing(RenderWindow& window) const
{
	CircleShape circle(get_radius());
	circle.setOutlineThickness(2.f);
	circle.setOutlineColor(Color(0, 180, 255));
	circle.setFillColor(Color(40, 200, 30));
	circle.move(get_x_position(), get_y_position());
	window.draw(circle);
}


// class Rectangle : public MyShape

Component* Rectangle::search_by_coord(Component* max_nesting_component, float x, float y)
{
	if (x_position + width >= x && x_position <= x)
	{
		if (y_position + height >= y && y_position <= y)
		{
			return this;
		}
	}
	return max_nesting_component;
}

Component* Rectangle::copy_elem()
{
	Component* component = new Rectangle;
	general_copy(component);
	component->set_parent(get_parent());
	return component;
}

void Rectangle::information_output()
{
	if (outputting_flag == false)
	{
		outputting_flag = true;
		nesting_output();
		cout << name << " (" << x_position << ", " << y_position << ") width = " << width << " height = " << height << endl;
	}
}

void Rectangle::drawing(RenderWindow& window) const
{
	RectangleShape rectangle(Vector2f(get_width(), get_height()));
	rectangle.setOutlineThickness(2.f);
	rectangle.setOutlineColor(Color(0, 180, 255));
	rectangle.setFillColor(Color(20, 10, 150));
	rectangle.move(get_x_position(), get_y_position());
	window.draw(rectangle);
}


//class MyGroup : public Component

void MyGroup::group_increase(Component* component)
{
	if (x_position == 0 && y_position == 0)
	{
		x_position = component->get_x_position() - 5;
		y_position = component->get_y_position() - 5;
		height = component->get_height() + 10;
		width = component->get_width() + 10;
	}
	else
	{
		if (x_position >= component->get_x_position())
		{
			width += (x_position - component->get_x_position()) + 8;
			x_position = component->get_x_position() - 4;
		}
		if (x_position + width <= component->get_x_position() + component->get_width())
		{
			width += (component->get_x_position() + component->get_width() - (x_position + width)) + 8;
		}

		if (y_position >= component->get_y_position())
		{
			height += (y_position - component->get_y_position()) + 8;
			y_position = component->get_y_position() - 4;
		}
		if (y_position + height <= component->get_y_position() + component->get_height())
		{
			height += (component->get_y_position() + component->get_height() - (y_position + height)) + 8;
		}
	}
}

void MyGroup::add(Component* component)
{
	components.push_back(component);
	component->set_parent(this);
	group_increase(component);
}
void MyGroup::remove(Component* component)
{
	component->set_parent(nullptr);
	components.remove(component);
}

void MyGroup::relocating(float x, float y)
{
	x_position -= x;
	y_position -= y;

	Iterator<Component, list <Component*>>* it = create_iterator();
	for (it->First(); !it->IsDone(); it->Next())
	{
		it->Current()->relocating(x, y);
	}
}

void MyGroup::remove_elem(Component* component)
{
	invalidate_element(component);
	components.remove(component);
}

void MyGroup::invalidate_additional_parameters(Component* component)
{
	Iterator<Component, list <Component*>>* it = create_iterator();
	for (it->First(); !it->IsDone(); it->Next())
	{
		invalidate_element(it->Current());
	}
}

Component* MyGroup::copy_elem()
{
	Component* component = new MyGroup;
	general_copy(component);
	component->set_parent(get_parent());

	for (auto comp : components)
	{
		component->add(comp->copy_elem());
	}

	return component;
}

void MyGroup::information_output()
{
	if (outputting_flag == false)
	{
		outputting_flag = true;
		if (name == "Root") { cout << name << endl; }
		else
		{
			nesting_output();
			cout << name << " (" << x_position << ", " << y_position << ") width = " << width << " height = " << height << endl;
		}
		Iterator<Component, list <Component*>>* it = create_iterator();
		for (it->First(); !it->IsDone(); it->Next())
		{
			it->Current()->information_output();
		}
		delete it;
	}
}

Component* MyGroup::search_by_coord(Component* max_nesting_component, float x, float y)
{
	int max_nesting = -1;
	bool area_flag = false;
	if (get_parent())
	{
		if (x_position + width >= x && x_position <= x)
		{
			if (y_position + height >= y && y_position <= y)
			{
				area_flag = true;
				max_nesting_component = this;
			}
		}
	}

	if (!get_parent() || area_flag)
	{
		for (auto component : components)
		{
			if (max_nesting <= component->nesting_cnt())
			{
				max_nesting = component->nesting_cnt();
				max_nesting_component = component->search_by_coord(max_nesting_component, x, y);
			}
		}
	}
	if (max_nesting_component) { max_nesting_component->outputting_flag = false; }

	return max_nesting_component;
}

void MyGroup::free_memory()
{
	cout << "\nClearing the memory of the " << name;
	Iterator<Component, list <Component*>>* it = create_iterator();
	for (it->First(); !it->IsDone(); it->Next())
	{
		it->Current()->free_memory();
	}
	delete this;
}

void MyGroup::drawing(RenderWindow& window) const
{
	if (parent)
	{
		RectangleShape group(Vector2f(get_width(), get_height()));
		group.setOutlineThickness(5.f);
		group.setOutlineColor(Color(180, 30, 30));
		group.setFillColor(Color(230, 0, 0));
		group.move(get_x_position(), get_y_position());
		window.draw(group);
	}
	for (auto component : components)
	{
		component->drawing(window);
	}
}