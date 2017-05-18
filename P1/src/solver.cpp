#include "solver.hpp"

Solver::Solver(Instance *problem_instance) {
    _problem_instance = problem_instance;
}

Result Solver::SetInitialN() {
    int n = _problem_instance->GetNTerminals();
    /* Add all labels (s, {s}) to N for s not the root terminal. */
    for (int i = 1; i < n; i++) {
        bitset<BITSET_SIZE> b;
        b.set(i);
        Vertex *s = _problem_instance->GetTerminals()[i];
        Label *l = new Label(s, b);
        l->SetL(0);
        s->AddLabel(l);
        _N.push(l);
    }
    cout << "Added " << _N.size() << " initial labels to N.\n";
    return SUCCESS;
}

Result Solver::SetInitialLabels() {
    int n = _problem_instance->GetNVertices();
    bitset<BITSET_SIZE> b;
    
    for (int i = 0; i < n; i++) {
        Vertex *v = _problem_instance->GetV()[0][i];
        Label *l = new Label(v, b);
        l->SetL(0);
        l->SetInP();
        v->AddLabel(l);
    }
    return SUCCESS;   
}

Result Solver::SolveCurrentInstance() {
    SetInitialN();
    SetInitialLabels();
    Label *current_label;
    bitset<BITSET_SIZE> final_terminal_set;

    while (_N.size() > 0) {
        current_label = _N.top(); _N.pop();
        current_label->SetInP();
        cout << "Considering \n";
        ConsiderNeighbours(current_label);
        current_label->Print();
        cout << "Merging \n";
        
        Merge(current_label);

    }

    bitset<BITSET_SIZE> b;
    for (int i = 1; i < _problem_instance->GetNTerminals(); i++)
        b.set(i);
    cout << b;
    int index = _problem_instance->GetTerminals()[0]->_label_hash[b];
    Label *l = _problem_instance->GetTerminals()[0]->_labels[index];
    l->Print();
    cout << l->GetL();

    // bitset<BITSET_SIZE> *k = l->GetBitset();
    // cout << _problem_instance->GetTerminals()[2]->_label_hash[*k];

    return SUCCESS;
}

/* l = (v, I) */
Result Solver::ConsiderNeighbours(Label *v_label) {
    Vertex *v = v_label->GetVertex();
    bitset<BITSET_SIZE> b = *v_label->GetBitset();

    /* Loop over neighbours of v */
    for (int i = 0; i < v->GetNNeigh(); i++) {
        Vertex *w = v->GetNeigh()[0][i];
        
        auto it = w->GetLabelHash()[0].find(b);
        /* (w, I) not yet set so set it and add it to _N and the label array of w */
        if (it == w->GetLabelHash()[0].end()) {
            Label *w_label = new Label(w, b);
            w_label->SetL(v_label->GetL() + RectDistance(v, w));
            w->AddLabel(w_label);
            _N.push(w_label);

        }
        /* (w, I) already set, check if l(v, I) + c({v, w}) < l(w, I) and 
         * if so replace l(w, I) by this value and add (w, I) to _N */
        else {
            cout << "eLSE \n";
            int w_label_ind = it->second;
            cout << w_label_ind;
            cout << "\n";
            Label *w_label = w->GetLabels()[0][w_label_ind];
            
            if (!w_label->IsInP() && 
                v_label->GetL() + RectDistance(v,w) < w_label->GetL()) {

                w_label->SetL(v_label->GetL() + RectDistance(v,w));

                _N.push(w_label);
            }
        }
    }
    return SUCCESS;
}

Result Solver::Merge(Label *first_label) {
    /* Loop over all labels in v._labels */
    Vertex* v = first_label->GetVertex();
    bitset<BITSET_SIZE> b1 = first_label->GetBitset()[0];
    vector<Label*> labels = v->GetLabels()[0];
    for (unsigned int i = 0; i < labels.size(); i++) {
        Label *second_label = labels[i];
        bitset<BITSET_SIZE> b2 = second_label->GetBitset()[0];
        
        /* J is not empty and does not contain root */
        if (b2.count() > 0  && !b2.test(0)) {
            bitset<BITSET_SIZE> b12 = b1 | b2;
            auto it = v->GetLabelHash()[0].find(b12);
            /* If (v, IuJ) not yet set, set it and add it to _N and 
             * the label array of v */
            if (it == v->GetLabelHash()[0].end()) {
                Label *new_label = new Label(v, b12);
                new_label->SetL(first_label->GetL() + second_label->GetL());
                v->AddLabel(new_label);
                _N.push(new_label);
            }
            /* If (v, IuJ) already set, check if l(v, I) + l(v, J) < l(v, IuJ) and 
            * if so replace l(v, IuJ) by this value and add (v, IuJ) to _N */
            else {
                int IJ_label_ind = it->second;
                Label *IJ_label = v->GetLabels()[0][IJ_label_ind];
                if (!IJ_label->IsInP() &&
                    first_label->GetL() + second_label->GetL() < IJ_label->GetL()) {
                    IJ_label->SetL(first_label->GetL() + second_label->GetL());
                    _N.push(IJ_label);
                }
            }
            
        }


    }
    return SUCCESS;
}

// TODO: test.
int RectDistance(Vertex *v, Vertex *w) {
    return  (abs(v->getX() - w->getX()) +
            abs(v->getY() - w->getY()) +
            abs(v->getZ() - w->getZ()));
}