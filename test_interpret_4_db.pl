motherchild(trude, sally).
 
fatherchild(tom, sally).
fatherchild(tom, erica).
fatherchild(mike, tom).
 
sibling(X, Y)      :- parentchild(Z, X), parentchild(Z, Y).
 
parentchild(X, Y) :- fatherchild(X, Y).
parentchild(X, Y) :- motherchild(X, Y).

parent(chester,irvin).
parent(chester,clarence).
parent(chester,mildred).
parent(irvin,ron).
parent(irvin,ken).
parent(clarence,shirley).
parent(clarence,sharon).
parent(clarence,charlie).
parent(mildred,mary).

male(chester).
female(mildred).
male(irvin).
female(shirley).
male(clarence).
female(sharon).
male(ron).
female(mary).
male(ken).
male(charlie).

father(X,Y) :- parent(X,Y), male(X).
grandparent(X,Y) :- parent(X,Z), parent(Z,Y).
paternalgrandfather(X,Y) :- father(X,Z), father(Z,Y).
sibling(X,Y) :- parent(Z,X), parent(Z,Y).



