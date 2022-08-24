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

	Iterator(U& children_);

	void	First();
	void	Next();
	bool	IsDone();
	T*		Current();

private:
	U&		children;
	it_type it;
};


class Component
{
public:
	bool outputting_flag { false };

	friend class Iterator<Component, list <Component*>>;

	Component();
	~Component();

	void				set_parent(Component* _parent);
	virtual void		set_radius(float _radius);
	
	Component*			get_parent()		const;
	string				get_name()			const;
	float				get_x_position()	const;
	float				get_y_position()	const;
	float				get_height()		const;
	float				get_width()			const;
	virtual float		get_radius()		const;

	virtual void		add(Component* component);
	virtual void		remove(Component* component);
	virtual Component*	clone()	const;
	void				general_copy(Component* component) const; // copying general parameters
	virtual Component*	copy_elem(); // copy the entire element

	virtual void		relocating(float x, float y);
	void				invalidate_element(Component* component);
	virtual void		invalidate_additional_parameters(Component* component);
	virtual void		remove_elem(Component* component);
	void				repositioning(float x, float y); // set a new element position
	void				relocate_elem(Component* component); // set a new element location
	virtual Component*	search_by_coord(Component* max_nesting_component, float x, float y);

	virtual bool		is_composite();
	int					nesting_cnt();
	void				nesting_output();

	virtual void		drawing(RenderWindow& window) const;
	virtual void		information_output();
	virtual void		free_memory();

protected:
	Component*	parent { nullptr };
	string		name;

	float		x_position { 0 };
	float		y_position { 0 };
	float		height { 0 };
	float		width { 0 };

	bool		nesting_flag { false };
};


class MyShape : public Component
{
public:
	void remove(Component* component)		override;
	void remove_elem(Component* component)	override;
	void relocating(float x, float y)		override;
	bool is_composite();
	void free_memory();
};


class Circle : public MyShape
{
public:
	Circle();
	Circle(float _radius, float x, float y);
	~Circle();

	void		set_radius(float _radius) override;
	float		get_radius() const override;

	Component*	clone() const override;
	Component*	copy_elem() override;
	void		copy_additional_parameters(Component* component);

	void		invalidate_additional_parameters(Component* component) override;
	Component*	search_by_coord(Component* max_nesting_component, float x, float y)	override;

	void		drawing(RenderWindow& window) const override;
	void		information_output() override;

private:
	float radius;
};


class Rectangle : public MyShape
{
public:
	Rectangle();
	Rectangle(float _height, float _width, float x, float y);
	~Rectangle();

	Component*	clone() const override;
	Component*	copy_elem() override;

	Component*	search_by_coord(Component* max_nesting_component, float x, float y) override;

	void		drawing(RenderWindow& window) const override;
	void		information_output() override;
};


class MyGroup : public Component
{
public:
	MyGroup();
	MyGroup(const string _name);
	~MyGroup();

	virtual Iterator<Component, list <Component*>>* create_iterator()
	{
		return new Iterator<Component, list <Component*>>(this->components);
	}
	
	void		add(Component* component)			override;
	void		remove(Component* component)		override;
	Component*	clone()						const	override;
	Component*	copy_elem()							override;
	void		remove_elem(Component* component)	override;

	void		group_increase(Component* component); // set new dimensions for the group
	void		invalidate_additional_parameters(Component* component) override;
	void		relocating(float x, float y) override;
	Component*	search_by_coord(Component* max_nesting_component, float x, float y) override;

	bool		is_composite();

	void		drawing(RenderWindow& window) const override;
	void		information_output() override;
	void		free_memory();

protected:
	list <Component*> components;
};

#endif // ELEMENTS_H