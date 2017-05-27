#include "solver.hpp"

int labelcounter = 0;

Result Solver::SetGlobalUpperBound() {
    bitset<BITSET_SIZE> all_terminals;
    for (int i = 0; i < _problem_instance->GetNTerminals(); i++) {
        all_terminals.set(i);
    }
    _global_upper_bound = _lower_bound_comp->MST(all_terminals);
    return SUCCESS;
}

/* Add a label to the priority queue _N and
 * compute the lower bound for the label 
 * if this hasn't happened before. */
Result Solver::AddLabelToN(Label* l) {
    if (!l->IsLowerBoundSet()) {
        int MST_bound = _lower_bound_comp->OneTreeLowerBound(l);
        int BB_bound = _lower_bound_comp->BBLowerBound(l);
        int lower_bound = max(BB_bound, MST_bound);
        l->SetLowerBound(lower_bound); 
    }

    /* Don't add labels that certainly won't contribute to a solution:
     * see Lemma 14 and 15. */
    if (l->GetL() <= _global_upper_bound && 
        !_lower_bound_comp->CompareToUpperBound(
                                    l->GetBitset()[0], l->GetL())) {
    _N.push(make_pair(l->GetL() + l->GetLowerBound(), l));
    }

    return SUCCESS;
}

/* Add (s, {s}) to _N for each terminal s unequal to 
 * the root. */
Result Solver::SetInitialN() {
    int n = _problem_instance->GetNTerminals();
    for (int i = 1; i < n; i++) {
        bitset<BITSET_SIZE> b;
        b.set(i);
        
        Vertex *s = _problem_instance->GetTerminals()[i];
        Label *l = new Label(s, b);
        labelcounter ++;
        
        l->SetL(0);
        /* Try to add label to N, if this succeeds add it to v. */
        if (AddLabelToN(l) == SUCCESS)
            s->AddLabel(l);

    }
    return SUCCESS;
}

/* Add (s, emptyset) for all vertices s with 
 * l(s, emptyset) = 0 and put these labels in P. */
Result Solver::SetInitialLabels() {
    int n = _problem_instance->GetNVertices();
    bitset<BITSET_SIZE> b;
    
    for (int i = 0; i < n; i++) {
        Vertex *v = _problem_instance->GetV()[0][i];
        Label *l = new Label(v, b);
        labelcounter ++;
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
    bitset<BITSET_SIZE> I = *v_label->GetBitset();

    /* Loop over neighbours of v */
    for (int i = 0; i < v->GetNNeighbours(); i++) {
        Vertex *w = v->GetNeighbours()[i];
        
        /* (w, I) not yet set so set it and add it to _N 
         * and the label array of w */
        Label *w_label;
        if ((w_label = w->GetLabelByBitset(I)) == NULL) {
            w_label = new Label(w, I);
            labelcounter ++;
            w_label->SetL(v_label->GetL() + RectDistance(v, w));
            /* Try to add label to N, if this succeeds add it to v. */
            if (AddLabelToN(w_label) == SUCCESS)
                w->AddLabel(w_label);
        }
        /* (w, I) already set, check if l(v, I) + d(v, w) < l(w, I) and 
         * if so replace l(w, I) by this value and add (w, I) to _N */
        else {
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
    bitset<BITSET_SIZE> I = I_label->GetBitset()[0];
    vector<Label*> labels = v->GetLabels();

    for (unsigned int i = 0; i < labels.size(); i++) {
        Label *J_label = labels[i];
        bitset<BITSET_SIZE> J = J_label->GetBitset()[0];
        
        /* (v,J) should be in P, J should be non-empty, not 
         * contain root and have no terminals in common with I. */
        if (
            J_label->IsInP()    &&
            J.count() > 0       && 
            !J.test(0)          &&
            (I & J).none())  {

            bitset<BITSET_SIZE> IJ = I | J;

            /* If (v, IuJ) not yet set, set it and add it to _N and 
             * the label array of v. In this case (v, IuJ) is not in P */
            Label *IJ_label;
            if ((IJ_label = v->GetLabelByBitset(IJ)) == NULL) {    
                IJ_label = new Label(v, IJ);
                labelcounter ++;
                IJ_label->SetL(I_label->GetL() + J_label->GetL());
    
                _lower_bound_comp->MergeUpperBound(I, J);
                
                /* Try to add label to N, if this succeeds add it to v. */
                if (AddLabelToN(IJ_label) == SUCCESS)
                    v->AddLabel(IJ_label);
            }
            /* If (v, IuJ) already set, check if l(v, I) + l(v, J) < l(v, IuJ)
             * and l(v, IuJ) is not in P. If so replace l(v, IuJ) by this 
             * value and add (v, IuJ) to _N */
            else {
                if (!IJ_label->IsInP() &&
                    I_label->GetL() + J_label->GetL() < IJ_label->GetL()) {
     
                    _lower_bound_comp->MergeUpperBound(I, J);
     
                    IJ_label->SetL(I_label->GetL() + J_label->GetL());
                    AddLabelToN(IJ_label);
                }
            }
        }
    }
    return SUCCESS;
}

/* Constructor / Destructor. */
Solver::Solver(Instance *problem_instance) {
    _problem_instance = problem_instance;
    _solution_found = false;

    _lower_bound_comp = new BoundComputator(problem_instance);
    SetGlobalUpperBound();
}

Solver::~Solver() {
    delete _lower_bound_comp;
}

/* Attempt to solve the given instance */
Result Solver::SolveCurrentInstance() {
    /* Set the root terminal (which is always just the
     * first one given) and the final terminal set 
     * (i.e. the set containing all terminals but the root). */
    Vertex *root = _problem_instance->GetTerminals()[0];
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

        /* Fetch the highest piority label from _N */
        current_label = _N.top().second; 
        _N.pop();


        // if (!(iteration_counter % 1)) {
        //     cout << "Iteration: " << iteration_counter << "\n";
        //     cout << "Size of priority queue: " << _N.size() << "\n";
        //     cout << "Labels created: " << labelcounter << "\n";
        //     cout << "Current Label value: " << current_label->GetL() + 
        //         current_label->GetLowerBound() << "\n\n";
        // }

        _lower_bound_comp->UpdateUpperBound(current_label);

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

    Label *l;
    if ((l = root->GetLabelByBitset(final_terminal_set)) != NULL) {
        _solution_value = l->GetL();
        _solution_found = true;
    }
    else {
        cout << "ERROR: N empty before solution was found!\n";
        return FAIL;
    }

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

void Solver::Test() {
    // bitset<BITSET_SIZE> I;
    // I.set(2);
    // I.set(1);

    // cout << _lower_bound_comp->ComplementDistance(I) << "\n";
}