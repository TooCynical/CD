/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * solver.cpp
 *
 *
 * Implementation of functionality described in solver.hpp.
 */

#include "solver.hpp"

Result Solver::SetGlobalUpperBound() {
    bitset<BITSET_SIZE> all_terminals;
    for (int i = 0; i < _problem_instance->GetNTerminals(); i++) {
        all_terminals.set(i);
    }
    _global_upper_bound = _bound_comp->MST(all_terminals);
    return SUCCESS;
}

Result Solver::AddLabelToN(Label* l) {
    if (!l->IsLowerBoundSet())
        l->SetLowerBound(_bound_comp->GetLowerBound(l)); 

    /* Don't add labels that certainly won't contribute to a solution:
     * see Lemma 14 and 15 in Hougardy et al. */
    if (l->GetL() > _global_upper_bound) {
        return FAIL;
    }
    if (_bound_comp->CompareToUpperBound(l)) {
        return FAIL;
    }

    _N.push(make_pair(l->GetL() + l->GetLowerBound(), l));
    return SUCCESS;
}

Result Solver::SetInitialN() {
    int n = _problem_instance->GetNTerminals();
    for (int i = 1; i < n; i++) {
        bitset<BITSET_SIZE> b;
        b.set(i);
        
        Vertex *s = _problem_instance->GetTerminals()[i];
        Label *l = new Label(s, b);
        
        l->SetL(0);
        if (AddLabelToN(l) == SUCCESS)
            s->AddLabel(l);
        else
            delete l;
    }
    return SUCCESS;
}

Result Solver::SetInitialLabels() {
    int n = _problem_instance->GetNVertices();
    bitset<BITSET_SIZE> b;
    
    for (int i = 0; i < n; i++) {
        Vertex *v = _problem_instance->GetVertices()[i];
        Label *l = new Label(v, b);
        l->SetL(0);
        l->SetInP();
        v->AddLabel(l);
    }
    return SUCCESS;
}

Result Solver::ConsiderNeighbours(Label *v_label) {
    Vertex *v = v_label->GetVertex();
    bitset<BITSET_SIZE> I = v_label->GetBitset();

    /* Loop over neighbours of v. */
    for (int i = 0; i < v->GetNNeighbours(); i++) {
        Vertex *w = v->GetNeighbours()[i];
        
        /* (w, I) not yet set so set it and add it to N 
         * and the labels of w. */
        Label *w_label;
        if ((w_label = w->GetLabelByBitset(I)) == NULL) {
            w_label = new Label(w, I);
            w_label->SetL(v_label->GetL() + RectDistance(v, w));
            /* Try to add label to N. If this succeeds add it to v.
             * If not, delete label. */
            if (AddLabelToN(w_label) == SUCCESS)
                w->AddLabel(w_label);
            else
                delete w_label;
        }
        /* (w, I) already set, check if l(v, I) + d(v, w) < l(w, I) and 
         * if so replace l(w, I) by this value and add (w, I) to N. */
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

Result Solver::Merge(Label *I_label) {
    Vertex* v = I_label->GetVertex();
    bitset<BITSET_SIZE> I = I_label->GetBitset();
    vector<Label*> labels = v->GetLabels();

    /* Loop over all labels associated with v. In the
     * implementation by Hougardy et al. the loop runs
     * over all valid (v, J) instead (regardless of whether 
     * they are associated with v yet) if this allows us to 
     * consider fewer options. We choose not to implement
     * this as it is very rare. Furthermore, we know that the 
     * label (v, emptyset) is always at index 0 in the vector, 
     * so instead of explicetely testing if J is empty, we simply
     * start at index 1. */
    unsigned int labels_size = labels.size();
    for (unsigned int i = 1; i < labels_size; i++) {
        Label *J_label = labels[i];
        bitset<BITSET_SIZE> J = J_label->GetBitset();
        
        /* (v,J) should be in P, J should be non-empty, not 
         * contain root and have no terminals in common with I.
         * Since J should not contain root anyways, we do not check this. */
        if (J_label->IsInP() && (I & J).none()) {
            bitset<BITSET_SIZE> IJ = I | J;

            /* If (v, I u J) not yet set, set it and add it to N and 
             * the label array of v. In this case (v, I u J) is not in P */
            Label *IJ_label;
            if ((IJ_label = v->GetLabelByBitset(IJ)) == NULL) {    
                IJ_label = new Label(v, IJ);
                IJ_label->SetL(I_label->GetL() + J_label->GetL());
    
                /* Attempt to improve the local upper bound for I u J. */
                _bound_comp->MergeUpperBound(I, J);
                
                /* Try to add label to N. If this succeeds add it to v. 
                 * If not, delete label. */
                if (AddLabelToN(IJ_label) == SUCCESS)
                    v->AddLabel(IJ_label);
                else
                    delete IJ_label;
            }
            /* If (v, IuJ) already set, check if l(v, I) + l(v, J) < l(v, IuJ)
             * and l(v, IuJ) is not in P. If so replace l(v, IuJ) by this 
             * value and add (v, IuJ) to N */
            else {
                if (!IJ_label->IsInP() &&
                    I_label->GetL() + J_label->GetL() < IJ_label->GetL()) {
     
                    /* Attempt to improve the local upper bound for I u J. */
                    _bound_comp->MergeUpperBound(I, J);
     
                    IJ_label->SetL(I_label->GetL() + J_label->GetL());
                    AddLabelToN(IJ_label);
                }
            }
        }
    }
    return SUCCESS;
}

Solver::Solver(Instance *problem_instance, BoundComputator *bound_comp) {
    _problem_instance = problem_instance;
    _bound_comp = bound_comp;

    SetGlobalUpperBound();
}

Result Solver::SolveCurrentInstance(int &ret) {
    /* Set the root terminal (which is always just the
     * first one given) and the final terminal set 
     * (i.e. the set containing all terminals but the root). */
    Vertex *root = _problem_instance->GetTerminals()[0];
    _problem_instance->GetTerminals()[0]->SetRoot();
    bitset<BITSET_SIZE> final_terminal_set;
    for (int i = 1; i < _problem_instance->GetNTerminals(); i++)
        final_terminal_set.set(i);

    /* Add (s, {s}) to _N for each terminal s unequal to 
     * the root, and set (s, emptyset) for all vertices s. 
     * By calling SetInitialLabels first, we guarantee that
     * the label (v, emptyset) is always at index 0 in the 
     * vector containing the labels of v. */
    SetInitialLabels();
    SetInitialN();

    Label *current_label;

    while (_N.size() > 0) {
        /* Fetch the highest piority label from N */
        current_label = _N.top().second; 
        _N.pop();

        /* Update the local upper bound for I. */
        _bound_comp->UpdateUpperBound(current_label);

        /* Attempt to prune according to Lemma 15. */
        if (_bound_comp->CompareToUpperBound(current_label))
            continue;

        /* Add label to P. If it already was in P, then this is 
         * a token label (i.e. it is copy left over from when
         * the priority value of the label was changed),
         * and we should ignore it */
        if (current_label->SetInP() == FAIL)
            continue;

        /* Check if current label = (root, terminals - {root})
         * in which case we are done */
        if (current_label->GetVertex()->IsRoot() && 
            current_label->GetBitset() == final_terminal_set)
            break;

        /* Perform the Dijkstra-step for each neighbour */
        ConsiderNeighbours(current_label);
        
        /* Perform the merge-step */
        Merge(current_label);    
    }

    /* Try and find l(root, R - {root}) and return it if found. */
    Label *l;
    if ((l = root->GetLabelByBitset(final_terminal_set)) != NULL) {
        ret = l->GetL();
        return SUCCESS;
    }
    else {
        cout << "ERROR: N empty before solution was found!\n";
        return FAIL;
    }
}
