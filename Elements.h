#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <SFML/Graphics.hpp>
#include <stdbool.h>
#include <iostream>
#include <string>
#include <list>

using namespace sf;
using namespace std;


template <typename T, typename U>
class Iterator
{
public:
	typedef typename list<T*>::iterator it_type;

	Iterator(U& children_, bool reverse = false) : children(children_)
	{
		it = children.begin();
	}
	void First() { it = children.begin(); }
	void Next() { ++it; }
	bool IsDone() { return (it == children.end()); }
	T* Current() { return *it; }

private:
	U& children;
	it_type it;
};

class Component
{
public:
	bool outputting_flag{ false };

	friend class Iterator<Component, list <Component*>>;

	Component() { name = "Root"; }
	~Component() {}

	void set_parent(Component* _parent) { parent = _parent; } // setters
	virtual void set_radius(float _radius) { }
	
	Component* get_parent() const { return parent; } // getters
	float get_x_position() const { return x_position; }
	float get_y_position() const { return y_position; }
	float get_height() const { return height; }
	float get_width() const { return width; }
	string get_name() const { return name; }
	virtual float get_radius() const { return 0; }

	virtual void add(Component* component) { }
	virtual void remove(Component* component) { }

	virtual void drawing(RenderWindow& window) const { }

	virtual bool is_composite() { return true; } // is it a composite element

	virtual Component* clone() const { return new Component(*this); }

	virtual void relocating(float x, float y) { }
	void repositioning(float x, float y); // set a new element position

	void relocate_elem(Component* component); // set a new element location 

	int nesting_cnt();
	void nesting_output();

	void invalidate_element(Component* component); // invalidate the element
	virtual void invalidate_additional_parameters(Component* component) { }
	virtual void remove_elem(Component* component) { } // delete element and its data
	
	void general_copy(Component* component) const; // function for copying general parameters
	virtual Component* copy_elem() { return nullptr; } // copy the entire element

	virtual Component* search_by_coord(Component* max_nesting_component, float x, float y) { return nullptr; }

	virtual void information_output() { } // data output
	virtual void free_memory() { }

protected:
	Component* parent{ nullptr };
	string name;

	float x_position{ 0 }, y_position{ 0 };
	float height{ 0 }, width{ 0 };

	bool nesting_flag{ false };
};


class MyShape : public Component
{
public:
	bool is_composite() { return false; }
	void relocating(float x, float y) override;
	void remove(Component* component) override;
	void remove_elem(Component* component) override;
	void free_memory();
};


class Circle : public MyShape
{
public:
	Circle() : radius(0) { name = "Circle"; }
	Circle(float _radius, float x, float y) : radius(_radius)
	{
		name = "Circle";
		x_position = x;
		y_position = y;
		height = width = radius * 2;
	}
	~Circle() {}


	void set_radius(float _radius) override { radius = _radius; }
	float get_radius() const override { return radius; }

	Component* clone() const override { return new Circle(*this); }

	void invalidate_additional_parameters(Component* component) override { radius = 0; }

	Component* search_by_coord(Component* max_nesting_component, float x, float y) override;

	void copy_additional_parameters(Component* component);
	Component* copy_elem() override;

	void information_output() override;
	void drawing(RenderWindow& window) const override;

private:
	float radius;
};


class Rectangle : public MyShape
{
public:
	Rectangle() { name = "Rectangle"; }
	Rectangle(float _height, float _width, float x, float y)
	{
		name = "Rectangle";
		x_position = x;
		y_position = y;
		height = _height;
		width = _width;
	}
	~Rectangle() {}

	
	Component* clone() const override { return new Rectangle(*this); }

	Component* search_by_coord(Component* max_nesting_component, float x, float y) override;

	Component* copy_elem() override;

	void information_output() override;
	void drawing(RenderWindow& window) const override;
};


class MyGroup : public Component
{
public:
	MyGroup() { name = "Group"; }
	MyGroup(const string _name)
	{
		name = _name;
	}
	~MyGroup() {}

	
	bool is_composite() { return true; }
	Component* clone() const override { return new MyGroup(*this); }

	void group_increase(Component* component); // set new dimensions for the group

	void add(Component* component) override;
	void remove(Component* component) override;

	virtual Iterator<Component, list <Component*>>* create_iterator()
	{
		return new Iterator<Component, list <Component*>>(this->components);
	}

	void relocating(float x, float y) override;

	void remove_elem(Component* component) override;
	void invalidate_additional_parameters(Component* component) override;

	Component* copy_elem() override;


	void information_output() override;

	Component* search_by_coord(Component* max_nesting_component, float x, float y) override;

	void drawing(RenderWindow& window) const override;
	void free_memory();

protected:
	list <Component*> components;

};

#endif