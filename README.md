<a id = "anchor_5"></a>
# Graphics editor

__Content list__
1. [About the program](#anchor_1)
2. [What is implemented in the program](#anchor_2)
3. [What technologies were used](#anchor_3)
4. [Some features of the code](#anchor_4)

---
<a id = "anchor_1"></a>
### About the program
In this project, the back-end logic of the graphic editor is implemented, which supports such shapes as a circle and a rectangle. These figures can be independent and combined into groups, which can also be combined with each other. Also, for clarity, the front-end part was implemented using the [SFML](https://www.sfml-dev.org/tutorials/2.5/) graphic library.

<a id = "anchor_2"></a>
### What is implemented in the program
>- *The word "Element" refers to both figures and groups*
>- *All operations take into account that groups can be non-empty*

- Adding an element
- Adding an element to a group
- Deleting an element
- Copy element
- Changing the position of an element
- Moving an element to another group or root
- Search for an element by coordinates (mouse selection). As a result, we get the element with the highest nesting, which is located at this point *Examples below*.

![image_3](https://user-images.githubusercontent.com/105476685/177790829-29c7269e-9546-477c-ad50-1de68e73e2d1.png)
![image_4](https://user-images.githubusercontent.com/105476685/177790835-6256b087-6017-4e49-ad0d-63b24afb9f3a.png)


<a id = "anchor_3"></a>
### What technologies were used
- Pattern builder
- Pattern iterator


<a id = "anchor_4"></a>
### Some features of the code
- The root for everything is a group, which must be created first and add unrelated elements to it
- Be careful when adding new items! Since they are added to the lists, they are drawn one by one. You need to go from large elements to smaller ones. *Examples below*.

``` C++

Component* tree = new MyGroup("Root");
Component* branch_1 = new MyGroup;
Component* branch_2 = new MyGroup;

Component* leaf_1 = new Circle(50, 30, 30);
Component* leaf_2 = new Rectangle(40, 20, 260, 360);
Component* leaf_3 = new Circle(40, 175, 175);


tree->add(branch_1); // large group
branch_1->add(leaf_1);
branch_1->add(leaf_2);

tree->add(branch_2); // then a small group
branch_2->add(leaf_3);
```

![image_1](https://user-images.githubusercontent.com/105476685/177788422-fc05c294-1204-4ee1-92b7-0e7af7bf47ac.png)

As a result, both groups are visible


As soon as we swap blocks with additions of elements
``` C++

tree->add(branch_2); // small group
branch_2->add(leaf_3);

tree->add(branch_1); // then a large group
branch_1->add(leaf_1);
branch_1->add(leaf_2);
```

![image_2](https://user-images.githubusercontent.com/105476685/177788431-55798fd6-2d0f-42f1-9f51-d9eda208dfbb.png)

We get that our smaller group is not visible in the window where the front-end is implemented, although everything is fine with the back-end part

[↑ UP ↑](#anchor_5)
