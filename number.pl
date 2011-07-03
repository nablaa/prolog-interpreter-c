%list(a, list(C)).
%list(C).
% Does not unify because of the occurrence of variable C.

%list(a, list(b, last)).
%animal(cat).
% Does not unify because the terms have a different name.

list(list(a, X), Y).
list(A, list(b, C)).
% Unifies with MGU {A = list(a, X), Y = list(b, C)}

%list(a, b).
%list(a, List).
%list(a, list(b, list(c, last))).
%list(a, List).
% Unifies with MGU {List = list(b, list(c, last))}

%number(1).
%number(Number).
% Unifies with MGU {Number = 1}.

%number(1).
%number(1).
% Unifies with an empty MGU.
%
