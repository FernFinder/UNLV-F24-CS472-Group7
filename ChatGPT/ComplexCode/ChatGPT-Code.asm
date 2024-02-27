#; Author: Ruben Langarica
#; Date Last Modified: 11/24
#; This program will explore the use of merge sort of a list of random numbers
#; this is done by using a calculation merge sort, that splits the array in 2
#; then the other helper function that will do the merging and arrage the numbers 
#; baised on if greater than or less than. This merge sort is baised on the c++ 
#; variation of temp arrays and not stacks.
.data
#;	System service constants
	SYSTEM_PRINT_INTEGER = 1
	SYSTEM_PRINT_STRING = 4
	SYSTEM_PRINT_CHARACTER = 11
	SYSTEM_READ_INTEGER = 5
	SYSTEM_EXIT = 10
	
#;	Random Number Generator Values
	M = 0x00010001
	A = 75
	C = 74
	previousRandomNumber: .word  1
	
	MINIMUM_VALUE = 1
	MAXIMUM_VALUE = 1000
	ARRAY_SIZE = 1000
	numbers: .space ARRAY_SIZE*4
	LEFTnumbers: .space ARRAY_SIZE*4	#;temp array
	RIGHTnumbers: .space ARRAY_SIZE*4	#;temp array
#;	Labels
	unsortedArrayLabel: .asciiz "Unsorted:\n"
	sortedArrayLabeL: .asciiz "\nSorted:\n"
	mergesLabel: .asciiz "\nMerges: "
	spaces: .asciiz " "
	newLine: .asciiz "\n"
	period: .asciiz "."
	Left: .asciiz "\nLeft:\n"
	Right: .asciiz "\nRight:\n"
.text
.globl main
.ent main
main:
	#;  uses the random number function to initialize the array to random values
	li $s2,0
	li $s3, ARRAY_SIZE
	la $s1, numbers
	
	InitializeLoop:
		li $a0, MINIMUM_VALUE	#;min value of 1
		li $a1, MAXIMUM_VALUE	#;max value of 100
		jal getRandomNumber		#;call randomnumber function
		move $s0,$v0			#;move random number to $s0

		sw $s0, ($s1)			#;move $s0 to array 
		
		addu $s1, $s1, 4		#;update to next address
		add $s2, $s2, 1			#;increment counter
	bne $s2, $s3, InitializeLoop#;if loop counter /= array_size

	#;	Prints the unsorted array of random values
	li $v0, SYSTEM_PRINT_STRING
	la $a0, unsortedArrayLabel
	syscall
	la $a0, numbers
	li $a1, ARRAY_SIZE
	jal printArray
	li $v0,0
	#;	Sort using mergeSort
	la $a0, numbers
	li $a1, 0				#;start location
	li $a2, 999				#;end location which is size -1
	jal mergeSort			#;call merge sort function
	move $s3, $v0			#;move merge count to $s3
	
	#;	Print the sorted array
	li $v0, SYSTEM_PRINT_STRING
	la $a0, sortedArrayLabeL
	syscall	
	la $a0, numbers
	li $a1, ARRAY_SIZE
	jal printArray

	#;--------------------------- used for debug
	
	#;li $v0, SYSTEM_PRINT_STRING
	#;la $a0, Left
	#;syscall	
	
	#;la $a0, LEFTnumbers
	#;li $a1, ARRAY_SIZE
	#;jal printArray
	
	#;li $v0, SYSTEM_PRINT_STRING
	#;la $a0, Right
	#;syscall	
	#;la $a0, RIGHTnumbers
	#;li $a1, ARRAY_SIZE
	#;jal printArray
	
	#;--------------------------------------

	#;	Print the number of merges used
	li $v0, SYSTEM_PRINT_STRING
	la $a0, mergesLabel
	syscall	
	
	li $v0, SYSTEM_PRINT_INTEGER
	move $a0, $s3
	syscall	
	
	li $v0, SYSTEM_EXIT
	syscall
.end main


#; Sorts an array using the merge sort algorithm:
#;The mergeSort function implements the Merge Sort algorithm in MIPS Assembly.
#;  Features
#;	Recursion: Divides the array into halves and recursively calls itself on each half.
#;	Merge Operation: Merges the sorted halves back together in a sorted manner.
#;	Base Case Handling: Terminates properly with a base case for arrays with one element.
#;	Merge Counting: Keeps track of the number of merges.
#;	Return a count of the number of merges
#;Arguments:
#;
#;    $a0: Address of the array (&array).
#;    $a1: Start index of the array.
#;    $a2: Middle index of the array.
#;    $a3: End index of the array.

.globl mergeSort
.ent mergeSort
mergeSort:
	subu $sp, $sp, 20
	sw $ra, 0($sp)		#;save return address because of recursion 
	sw $s0, 4($sp)		#;for start of array
	sw $s1, 8($sp)		#;for end of array
	sw $s2, 12($sp)		#;for the middle of array
	sw $s3, 16($sp)		#;for the merge count
	li $s3,0
	move $s0, $a1		#;save array left
	move $s1, $a2		#;save array right
	
	#;base case
	bge $s0, $s1, EndDivision		#;$s0 >= $s1 jump to EndDivision
		#;middle calculations
		sub $s2, $s1, $s0			#;(right-left)
		div $s2, $s2, 2				#;(right-left)/2
		add $s2, $s2, $s0			#;middle = left+(right-left)/2

		#;mergeSort(array,left,middle)       mergeSort($a0,$a1,$a2)
		move $a0, $a0			#;move array to $a0
		move $a1, $s0			#;move saved left to $a1
		move $a2, $s2			#;move saved middle to $a2
		jal mergeSort			#;recursion call
		
		#;mergeSort(array,middle+1,right)      mergeSort($a0,$a1,$a2)
		add $s2, $s2, 1		#;add 1 to off set middle
		move $a0, $a0		#;move array to $a0
		move $a1, $s2		#;move saved middle+1 to $a1
		move $a2, $s1		#;move saved right to $a2
		sub $s2, $s2, 1		#;move back middle offset
		jal mergeSort
		
		#;mergeSortPart(array,left,middle,right)   mergeSortPart($a0,$a1,$a2,$a3)
		move $a0, $a0		#;move array to $a0
		move $a1, $s0		#;move saved left to $a1
		move $a2, $s2		#;move saved middle to $a2
		move $a3, $s1		#;move saved right to $a3
		jal mergeSortPart	#;call helper function of mergesort


#;---------------------------debug-------------------------
		#;subu $sp, $sp, 4
		#;sw $a0, ($sp)
		#;	li $v0, SYSTEM_PRINT_CHARACTER
		#;	li $a0, '\n'
		#;	syscall

		#;	la $a0, numbers
		#;	li $a1, ARRAY_SIZE
		#;	jal printArray
		#;lw $a0, ($sp)
		#;addu $sp, $sp, 4
#;---------------------------debug-------------------------
		add $s3, $s3, 1
	EndDivision:

	#;for merge count
	add $s3, $s3, $v0
	move $v0, $s3

	#;Just return since there is 1 element, load all stack saved stuff
	MergeSortBaseCase:
	lw $ra, ($sp)
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	lw $s2, 12($sp)
	lw $s3, 16($sp)
	addu $sp,$sp,20
	
	jr $ra
.end mergeSort




#;The mergeSortPart function is a helper function for merging two halves during the Merge Sort algorithm.
#;Features
#;
#;    Array Partitioning: Divides the array into two parts for merging.
#;    Pointer Manipulation: Uses pointers for efficient array manipulation.
#;    Combination Loop: Combines the two halves in sorted order.
#;    Array Copying: Copies elements from temporary arrays back to the original array.
#;Arguments:

#;    $a0: Address of the array (&array).
#;    $a1: Start index of the left array.
#;    $a2: Middle index (end of left array).
#;    $a3: End index of the right array.

.globl mergeSortPart
.ent mergeSortPart
mergeSortPart:
	subu $sp, $sp, 4
	sw $ra, 0($sp)

	#; $t1 = numberOne
	#; $t2 = numberTwo
	#; $t3 = i
	#; $t4 = j
	#; $t5 = x
	#; $t6 = RIGHTnumbers
	#; $t7 = LEFTnumbers
	#; $t8 = temp
	#; $t9 = temp

	sub $t1, $a2, $a1		#;$t1= middle-left+1
	add $t1, $t1, 1
	sub $t2, $a3, $a2		#;$t2 = right-middle

	li $t3, 0				#;variable for loop
	la $t6, RIGHTnumbers
	la $t7, LEFTnumbers
	NumberoneLoop:
		bge $t3, $t1, EndNumberoneLoop		#; variable >=$t1 then jump

		addu $t8, $t3, $a1				#;update to next address array[left+i]
		mul $t8, $t8, 4					#;for *4 array index
		addu $t4, $a0, $t8				#;update to next address

		mul $t8, $t3, 4					#;for *4 array index
		addu $t5, $t8, $t7				#;update to next address	Left[i]
		
		lw $t8, ($t4)
		sw $t8, ($t5)			#;Left[i] = array[left+i];

		add $t3, $t3, 1			#;loop counter +1
	j NumberoneLoop			#;loop
	EndNumberoneLoop:

	li $t3, 0					#;variable for loop
	NumberTwoLoop:
		bge $t3, $t2, EndNumbertwoLoop			#; variable >=$t2 then jump

		addu $t8, $t3, $a2		#;update to next address array[middle+i+1]
		addu $t8, $t8, 1		#;adds the 1
		mul $t8, $t8, 4			#;size mul 
		addu $t4, $t8, $a0		#;update to next address

		mul $t8, $t3, 4			#;mul i by size
		addu $t5, $t8, $t6		#;update to next address	right[i]

		lw $t8, ($t4)			#;load word into $t8
		sw $t8, ($t5)			#;Left[i] = array[left+i];

		add $t3, $t3, 1			#;loop counter
		j NumberTwoLoop
	EndNumbertwoLoop:

	li $t3, 0			#; i variable
	li $t4, 0			#; j variable
	move $t5, $a1		#; x variable

	#;i>=$t1 && j>=$t2 jump
	CombinationLoop:
		bge $t3, $t1, CombinationLoopEnd
		bge $t4, $t2, CombinationLoopEnd

		mul $t8, $t3, 4			#;i * size
		addu $a1, $t8, $t7		#;update to next address	Left[i]	used to be $t6

		mul $t8, $t4, 4			#;j * size
		addu $a2, $t8, $t6		#;update to next address	Right[j]	used to be $t7

		mul $t8, $t5, 4			#;x * size
		addu $a3, $t8, $a0		#;update to next address	array[x]

		lw $a1, ($a1)			#;load word of left array
		lw $a2, ($a2)			#;load word of right array
		bgt $a1, $a2, ElseStatment
			sw $a1, ($a3)			#; array[x]=Left[i];
			add $t3, $t3, 1			#;increment i since not put in right side
			add $t5, $t5, 1			#;loop counter
		j CombinationLoop
		ElseStatment:
			sw $a2, ($a3)			#; array[x]=Right[j];
			add $t4, $t4, 1			#;increment j since not put in left side
			add $t5, $t5, 1			#;loop counter
		j CombinationLoop
	CombinationLoopEnd:

	CopyNumberOneLoop:
		bge $t3, $t1, EndCopyNumberOneLoop	#;if i>= numberone then end loop $t3>=$t1

		mul $t8, $t3, 4			#;i * size of data
		addu $a1, $t7, $t8		#;update to next address	Left[i]
		
		mul $t8, $t5, 4			#;x * size of data
		addu $t9, $a0, $t8		#;update to next address	array[x]

		lw $t8, ($a1)			#;load word of left[i]
		sw $t8, ($t9)			#;Array[x] = Left[i]

		add $t3, $t3, 1			#;i incrementation 
		add $t5, $t5, 1			#;x incrementation
	j CopyNumberOneLoop
	EndCopyNumberOneLoop:

	CopyNumberTwoLoop:
		bge $t4, $t2, EndCopyNumberTwoLoop

		mul $t8, $t4, 4			#;j * size of data
		addu $t3, $t8, $t6		#;update to next address	right[j]

		mul $t8, $t5, 4			#;x * size of data
		addu $t9, $t8, $a0		#;update to next address	array[x]

		lw $t8, ($t3)			#;load word of right[j]
		sw $t8, ($t9)			#;array[x] = right[j]

		add $t4, $t4, 1			#;j incrementation 
		add $t5, $t5, 1			#;x incrementation
	j CopyNumberTwoLoop
	EndCopyNumberTwoLoop:
	
	lw $ra, ($sp)
	addu $sp,$sp,4
	jr $ra
.end mergeSort
#; Gets a random non-negative number between a specified range

#; Returns a random signed integer number
.globl getRandomNumber
.ent getRandomNumber
getRandomNumber:
	lw $t0, previousRandomNumber	#;load word in $t0
	la $t1, previousRandomNumber	#;load address in $t0

	mulou $t2, $t0, A
	add $t2, $t2, C
	rem $t2, $t2, M
	sw $t2, ($t1)

	sub $t1, $a1, $a0
	add $t1, $t1, 1
	rem $t1, $t2, $t1
	
	add $t1, $t1, $a0
	move $v0, $t1
	#;rem $v0, $t2, $t1		#;return in $v0
	jr $ra
.end getRandomNumber

#; Prints an array to the console with a new line every 10 elements
#; Arguments:
#;	$a0 - &array
#;	$a1 - elements
.globl printArray
.ent printArray
printArray:
	move $t0, $a0
	move $t2, $a1

	li $t1, 0
	PrintLoop:
		li $v0, SYSTEM_PRINT_INTEGER
		lw $a0, ($t0)	
		syscall

		li $v0, SYSTEM_PRINT_STRING
		la $a0, spaces
		syscall

		addu $t0, $t0, 4		#;update to next address
		add $t1, $t1, 1			#;increment counter

		rem $t5, $t1, 10		
		bnez $t5, SkipNewLine
			li $v0, SYSTEM_PRINT_STRING
			la $a0, newLine
			syscall
		SkipNewLine:
	bne $t1, $t2, PrintLoop
	jr $ra
.end printArray