Title

  Goldbach-Geometry.cpp

Description

C++ program which can be used to compute
data supporting the conjecture in this paper.
Each option generates a user-specified output file
or options to send the output to the concole.

Input Options and output formats

  Start = srarting value of e
  End   = ending value of e

  Output option 1: f = output file
         option 2: c = console output

  Output options:

	a = for absolute detail
	c = convolution followed by fold level	
	d = for differences only
	e = verify induction step
	f = for first solution
	g = Goldbachs comet
	i = to iterate
	l = for max/min limits only
	m = modular form
	p = Gaussian primes
	s = for all solutions
	t = for total solutions only
	u = solution vector only
	v = verify solution
	x = exceptions only
	y = Geometry

Option 1: Output file
  Each output line consists of the pattern 
     {e,n, {p, e-p}}:
	  e is the even integer
	  n is the totla number of solutions
     p is the first prime solution

Option 2: Results in console from options selected.

Dependencies

  Self contained. Compile with any C++ compiler.

Author

  Michael Mezzino - michaelmezzino@comcast.net

Version History

  01 Initial release
