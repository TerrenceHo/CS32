void listAll(const MenuItem* m, string path) // two-parameter overload
{
    if(m->menuItems() == NULL or m->menuItems()->size() <= 0){
        return;
    }
    
    for (vector<MenuItem*>::const_iterator it = m->menuItems()->begin(); it != m->menuItems()->end(); it++){
        cout << path + (*it)->name() << endl;
        listAll((*it), path + (*it)->name() + "/");
    }
}

