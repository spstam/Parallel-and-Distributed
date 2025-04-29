from pprint import pprint
from numpy import array, zeros, diag, diagflat, dot, ones

def jacobi(A,b,N=2000,x=None):
	"""Solves the equation Ax=b via the Jacobi iterative method."""
	# Create an initial guess if needed
	if x is None:
		x = zeros(len(A[0]))

	# Create a vector of the diagonal elements of A
	# and subtract them from A
	D = diag(A)
	R = A - diagflat(D)

	# Iterate for N times
	for i in range(N):
		x = (b - dot(R,x)) / D
	return x

# Create array a with 2 in the diagonal and -1 in the off-diagonals with size n
n = 100
a = -1 * ones(n-1)
A = diag(a, -1) + diag(a, 1) + diag(2*ones(n))
b = zeros(n)
b[-1] = n+1
guess = zeros(n)

sol = jacobi(A,b,N=2000,x=guess)

print("A:")
pprint(A)

print("b:")
pprint(b)

print("x:")
pprint(sol)

# Calculate norm of residual
residual = b - dot(A, sol)
norm = dot(residual, residual)
print(f"Norm of residual: {norm}")
