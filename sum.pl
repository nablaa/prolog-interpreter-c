% Base 0+N = 0
sum(0, N, N).

% Simple recursion V+1 + N = V + N+1
sum(s(V), N, M) :-
    sum(V, s(N), M).

% Sample queries
%
% Query: sum(s(0), s(0), Sum).
% Yes.
% Solution: sum(s(0),s(0),s(s(0))).
%
% Query: sum(s(s(0)), Value, s(s(s(0)))).
% Yes.
% Solution: sum(s(s(0)),s(0),s(s(s(0)))).
% More solutions (y/n) ?
% y
% No.
%
% Query: sum(A, B, s(s(0))).
% Yes.
% Solution: sum(0,s(s(0)),s(s(0))).
% More solutions (y/n) ?
% y
% Yes.
% Solution: sum(s(0),s(0),s(s(0))).
% More solutions (y/n) ?
% y
% Yes.
% Solution: sum(s(s(0)),0,s(s(0))).
