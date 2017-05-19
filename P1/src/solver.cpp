#include "solver.hpp"

/* Add a label to the priority Q _N and
 * compute the lower bound for the label 
 * if this hasn't happened before. */
Result Solver::AddLabelToN(Label* l) {
    if (!l->IsLowerBoundSet()) {
        l->SetBeenInQ();
        int lower_bound = BBLowerBound(l, 
                                       _problem_instance->GetTerminals(),
                                       _problem_instance->GetNTerminals());
        l->SetLowerBound(lower_bound);
    }
    _N.push(l);
    return SUCCESS;
}

/* Add (s, {s}) to _N for each termianl s unequal to 
 * the root. */
Result Solver::SetInitialN() {
    int n = _problem_instance->GetNTerminals();
    for (int i = 1; i < n; i++) {
        bitset<BITSET_SIZE> b;
        b.set(i);
        
        Vertex *s = _problem_instance->GetTerminals()[i];
        Label *l = new Label(s, b);
        
        l->SetL(0);
        s->AddLabel(l);
        AddLabelToN(l);
    }
    return SUCCESS;
}

/* Add (s, emptyset) for all vertices s with 
 * l(s, emptyset) = 0. */
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

/* Consider for the given label (v, I) all neighbours
 * (w, I) and perform the Dijkstra-step if needed. */
Result Solver::ConsiderNeighbours(Label *v_label) {
    Vertex *v = v_label->GetVertex();
    bitset<BITSET_SIZE> b = *v_label->GetBitset();

    /* Loop over neighbours of v */
    for (int i = 0; i < v->GetNNeigh(); i++) {
        Vertex *w = v->GetNeigh()[0][i];
        
        auto it = w->GetLabelHash()[0].find(b);
        /* (w, I) not yet set so set it and add it to _N 
         * and the label array of w */
        if (it == w->GetLabelHash()[0].end()) {
            Label *w_label = new Label(w, b);
            w_label->SetL(v_label->GetL() + RectDistance(v, w));
            w->AddLabel(w_label);
            AddLabelToN(w_label);

        }
        /* (w, I) already set, check if l(v, I) + c({v, w}) < l(w, I) and 
         * if so replace l(w, I) by this value and add (w, I) to _N */
        else {
            int w_label_ind = it->second;
            Label *w_label = w->GetLabels()[0][w_label_ind];
            
            if (!w_label->IsInP() && 
                v_label->GetL() + RectDistance(v,w) < w_label->GetL()) {

                w_label->SetL(v_label->GetL() + RectDistance(v,w));

                AddLabelToN(w_label);
            }
        }
    }
    return SUCCESS;
}

/* Perform the merge-step for given label */
Result Solver::Merge(Label *I_label) {
    /* Loop over all labels in v._labels */
    Vertex* v = I_label->GetVertex();
    bitset<BITSET_SIZE> b1 = I_label->GetBitset()[0];
    vector<Label*> labels = v->GetLabels()[0];
    for (unsigned int i = 0; i < labels.size(); i++) {
        Label *J_label = labels[i];
        bitset<BITSET_SIZE> b2 = J_label->GetBitset()[0];
        
        /* J should be non-empty,  not contain root and have 
         * no terminals in common with J. */
        if (b2.count() > 0  && 
            !b2.test(0)     &&
            (b1 & b2).none()) {
            bitset<BITSET_SIZE> b12 = b1 | b2;
            auto it = v->GetLabelHash()[0].find(b12);
            /* If (v, IuJ) not yet set, set it and add it to _N and 
             * the label array of v. In this case (v, IuJ) is not in P */
            if (it == v->GetLabelHash()[0].end()) {
                Label *IJ_label = new Label(v, b12);
                IJ_label->SetL(I_label->GetL() + J_label->GetL());
                v->AddLabel(IJ_label);
                AddLabelToN(IJ_label);
            }
            /* If (v, IuJ) already set, check if l(v, I) + l(v, J) < l(v, IuJ)
             * and l(v, IuJ) is not in P. If so replace l(v, IuJ) by this 
             * value and add (v, IuJ) to _N */
            else {
                int IJ_label_ind = it->second;
                Label *IJ_label = v->GetLabels()[0][IJ_label_ind];
                if (!IJ_label->IsInP() &&
                    I_label->GetL() + J_label->GetL() < IJ_label->GetL()) {
                    IJ_label->SetL(I_label->GetL() + J_label->GetL());
                    AddLabelToN(IJ_label);
                }
            }
        }
    }
    return SUCCESS;
}

/* Constructor */
Solver::Solver(Instance *problem_instance) {
    _problem_instance = problem_instance;
    _solution_found = false;
}

/* Attempt to solve the given instance */
Result Solver::SolveCurrentInstance() {
    /* Set the root terminal (which is always just the
     * first one given) and the final terminal set 
     * (i.e. the set containing all terminals but the root). */
    _problem_instance->GetTerminals()[0]->SetRoot();
    bitset<BITSET_SIZE> final_terminal_set;
    for (int i = 1; i < _problem_instance->GetNTerminals(); i++)
        final_terminal_set.set(i);

    /* Add (s, {s}) to _N for each termianl s unequal to 
     * the root, and set (s, emptyset) for all vertices s. */
    SetInitialN();
    SetInitialLabels();

    Label *current_label;
    int iteration_counter = 0;

    while (_N.size() > 0) {
        iteration_counter ++;

        if (!(iteration_counter % 10000)) {
            cout << "Size of priority queue: " << _N.size() << "\n";
            cout << "Iteration: " << iteration_counter << "\n";
        }
        /* Fetch the highest piority label from _N */
        current_label = _N.top(); 
        _N.pop();

        /* Add label to P. If it already was in P, then this is 
         * a token label (i.e. it is copy left over from when
         * the priority value of the label was changed),
         * and we should ignore it */
        if (current_label->SetInP() == FAIL)
            continue;

        /* Check if current label = (root, terminals - {root})
         * in which case we are done */
        if (current_label->GetVertex()->IsRoot() && 
            current_label->GetBitset()[0] == final_terminal_set)
            break;

        /* Perform the Dijkstra-step for each neighbour */
        ConsiderNeighbours(current_label);
        
        /* Perform the merge-step */
        Merge(current_label);
    }

    cout << "Iteration: " << iteration_counter << "\n";

    /* To find the solution, get l(root, {Terminals - root}) */
    int index = _problem_instance->GetTerminals()[0]->
                        GetLabelHash()[0][final_terminal_set];
    Label *l = _problem_instance->GetTerminals()[0]->
                        GetLabels()[0][index];
    _solution_value = l->GetL();
    _solution_found = true;

    return SUCCESS;
}

/* Put solution to given instance in ret, 
 * if one has been found already */
Result Solver::GetSolution (int &ret) {
    if (_solution_found) {
        ret = _solution_value;
        return SUCCESS;
    }
    else {
        return FAIL;
    }
}
