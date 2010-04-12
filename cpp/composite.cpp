class Component {
public:
    virtual void Add(Component* parent);
    virtual void Remove(Component* parent);
};
void Component::Add(Component* parent){};
void Component::Remove(Component* parent){};

class Composite:public Component{
};

class NotComposite{
    virtual void Add(Component* parent);
    virtual void Remove(Component* parent);
};
void NotComposite::Add(Component* parent){};
void NotComposite::Remove(Component* parent){};

class Leaf:public Component, public NotComposite{
};


int main()
{
    Component* cmp;
    Leaf* lfp;
    Leaf lf;
    Composite cs;

    cmp = &lf;
    cmp->Add(cmp);
    cmp->Remove(cmp);

    lfp = &lf;

    lfp->Add(cmp);  //compile error
    lf.Add(cmp);    //compile error

    return 0;
}

