This folder contains an implementation of the Dijkstra-Steiner algorithm for 
the 3D rectilinear steiner tree problem.


Implemented functionality:
    Our implementation follows the one in Hougardy et al. closely. In particular, 
    we have implemented:
        - Parsing of instance files with somewhat extensive input sanitation.
        - The core Dijkstra-Steiner algorithm with future costs / lower bounds:
            - The bounding box lower bound.
            - The one-tree lower bound.
        - Pruning of labels via a global upper bound (as per Lemma 14 in Hougardy et al.)
            - Global upper bound via MST on terminal set.
        - Pruning of labels via local upper bounds (as per Lemma 15 in Hougardy et al.):
            - Direct update of local upper bound on I whenever a label of the form
              (v, I) is popped off the priority queue as described in section 5
              of Hougardy et al.
            - Merging of local upper bounds for I and J whenever two labels 
              (v, I) and (v, J) are merged (in the core algorithm).
    Wherever possible, results of computations are saved in various hash tables,
    and reused.

Documentation:
    Comments describing functionality of classes / methods / functions can be found in the header (.hpp) files in the /include/ folder. 
    The source (.cpp) files in the /src/ folder only contain comments refering to details of implementation of methods / functions.

Compilation:
    The code can be compiled using 'make'. Using 'make test' will assert 
    some test cases as well. The binary will be ./bin/main

Usage:
    ./bin/main <filename> [boundopts]
    
    where <filename> is a required argument that should point to a well-formatted instance
    file, and boundopts is an optional argument used to set which kind of lower bound should be used: 
        ALL     - Use maximum of Bounding box and one-tree lower bounds.
        OT      - Use one-tree lower bound only.
        BB      - Use bounding box lower bound only.
        NONE    - Use no lower bound.

Results:
    The code runs and (hopefully) works correctly, as far as we know there are no
    bugs, and there should be no memory leakages (except on termination with an 
    error).
    
    By default, we use the BB lower bound but not the onetree lower bound: the long 
    computation time of the onetree lower bound is not worth it.
    
    As for speed, we are able to come within a factor ~6 of the run speed achieved by
    Hougardy et al. on our CPU (which is comparable to theirs). This is based on one
    testfile only (with 40 terminals) so it should be taken as an indication only.
    Instances with 20 terminals are solved in about 1-30 seconds. Instances with 
    40 terminals take between 3-60 minutes.

Testing:
    The folder /testing/ contains several python / bash scripts which can be 
    used to test the program. This is probably not very interesting but we 
    included them anyways. Additionally, the folder contains a set of generated
    instances files which can also be used for testing.
