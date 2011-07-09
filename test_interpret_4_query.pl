sibling(sally, eric). % infinite loop!

parent(irvin, X).
parent(clarence,X), male(X).
parent(X,Y), parent(Y,sharon).
%paternalgrandfather(X,ken).
%paternalgrandfather(chester,X).



quit.
