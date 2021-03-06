
/**
 * Create a program that:
 * 1) May or may not take a single command line parameter to use as a seed to
 *    the random number generator (use atoi() to convert argv[1] to a integer
 *    feed the result to srand(3), but only if argc is > 1.
 * 2) Emulates rolling two 10 sided dice using the rand(3) function call and
 *    adding the result together. (should give a result between 2 and 20.
 * 3) Keep track of the results of 500 rounds of rolling the two dice.
 * 4) Print a sideways histogram plot of the results:
 * Example input/output:
 * ./h10
 *  2: #
 *  3: #########
 *  4: ##############
 *  5: #####################
 *  6: ###########################
 * ...
 * 18: ##################
 * 19: ############
 * 20: ######
 */

