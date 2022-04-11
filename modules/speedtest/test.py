
n = 0
fb_target = 100
fb_a = 1
fb_b = 1

while(n<fb_target):
	fb_a = fb_a + fb_b
	fb_b = fb_b + fb_a
	n = n+1
	print("Fib Numbers:",fb_a, fb_b)


