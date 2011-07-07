pet(cat).
person(john).
owns(john, cat).

owner(A, B) :-
	person(A),
	pet(B),
	owns(A, B).
