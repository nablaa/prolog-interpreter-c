sum(0, N, N).
sum(s(V), N, M) :- sum(V, s(N), M).

% Database: sum(0, N, N).
% Database: sum(s(V), N, M) :-
% Database:    sum(V, s(N), M).
% [program execution starts]
% Query: 
% sum(s(s(0)), s(0), Result).
% Solution: sum(s(s(0)),s(0),s(s(s(0)))).
% More solutions (y/n) ?
% n
% Query: 
% sum(Number, s(0), s(s(s(0)))).
% Solution: sum(s(s(0)),s(0),s(s(s(0)))).
% More solutions (y/n) ?
% n
% Query: 
% sum(First, Second, s(0)).
% Solution: sum(0,s(0),s(0)).
% More solutions (y/n) ?
% y
% Solution: sum(s(0),0,s(0)).
% More solutions (y/n) ?
% n
% Query: 
% quit.
% [program execution ends]

