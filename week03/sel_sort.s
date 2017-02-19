        .text
        .globl        main
main:
        addi        $sp, $sp, -412      
        sw      $ra, 408($sp)           
        sw      $s0, 404($sp)           
        sw      $s1, 400($sp)           
        move    $s0, $sp

        # Ask the OS to read a number and put it in $s1 = n
        li        $v0, 5                
        syscall
        move    $s1, $v0

        # Now read in the list
        move    $a0, $s0
        move    $a1, $s1
        jal     rd_lst

        # Now sort the list
        move    $a0, $s0
        move    $a1, $s1
        jal     selsrt

        # Now print the list
        move    $a0, $s0
        move    $a1, $s1
        jal     pr_lst

        # Prepare for return
        lw      $ra, 408($sp)
        lw      $s0, 404($sp)
        lw      $s1, 400($sp)
        addi    $sp, $sp, 412
        li      $v0, 10
        syscall

rd_lst: 
        # Setup
        addi    $sp, $sp, -4
        sw      $ra, 0($sp)

        # Main for loop
        move    $t0, $zero
rd_tst: bge     $t0, $a1, rddone
        li      $v0, 5
        syscall
        sll     $t1, $t0, 2
        add     $t1, $a0, $t1
        sw      $v0, 0($t1)
        addi    $t0, $t0, 1
        j       rd_tst
        
        # Prepare for return
rddone: lw      $ra, 0($sp)
        addi    $sp, $sp 4
        jr      $ra

pr_lst: 
        # Setup
        addi    $sp, $sp, -4
        sw      $ra, 0($sp)            

        # Main for loop
        move    $t2, $a0                
        move    $t0, $zero              
pr_tst: bge     $t0, $a1, prdone
        sll     $t1, $t0, 2             
        add     $t1, $t2, $t1           
        lw      $a0, 0($t1)             
        li      $v0, 1                  
        syscall

        # Print a space 
        la      $a0, space              
        li      $v0, 4                  
        syscall

        addi    $t0, $t0, 1             
        j       pr_tst                  
        
        # print a newline
prdone: 
        la      $a0, newln
        li      $v0, 4                  
        syscall

        # Prepare for return
        lw      $ra, 0($sp)             
        addi    $sp, $sp 4              
        jr      $ra                     

selsrt:
        # Setup
        addi    $sp, $sp, -24           
        sw      $ra, 20($sp)            
        sw      $s0, 16($sp)            
        sw      $s1, 12($sp)            
        sw      $s2,  8($sp)            
        sw      $s3,  4($sp)           
        sw      $s4,  0($sp)           

        # We'll be using $a0, $a1 in function calls
        move    $s0, $a0               
        move    $s1, $a1               

        # Main for loop
        addi    $s3, $s1, -1
        move    $s2, $zero
lp_tst: bge     $s2, $s3, lpdone

        # Find smallest element
        move    $a0, $s0
        move    $a1, $s2
        move    $a2, $s3
        jal     locmin
        move    $s4, $v0

        # Swap list[i] and list[j]
        move    $a0, $s0
        move    $a1, $s2
        move    $a2, $s4
        jal     swap

        # Increment i
        addi    $s2, $s2, 1
        j       lp_tst
        
        # Prepare for return
lpdone: lw      $ra, 20($sp)
        lw      $s0, 16($sp)
        lw      $s1, 12($sp)
        lw      $s2,  8($sp)
        lw      $s3,  4($sp)
        lw      $s4,  0($sp)
        addi    $sp, $sp, 24
        jr      $ra

        
locmin:
        # Get min_loc and list[min_loc]
        move    $t0, $a1
        sll     $t3, $t0, 2
        addu    $t3, $t3, $a0
        lw      $t5, 0($t3)

        # Start for loop
        addi    $t1, $a1, 1
lm_lp:  bgt     $t1, $a2, lmdone
        # Load list[k]
        sll     $t2, $t1, 2
        addu    $t2, $a0, $t2
        lw      $t4, 0($t2)
        bge     $t4, $t5, no_chg
        move    $t0, $t1
        move    $t5, $t4

no_chg: addi    $t1, $t1, 1
        j       lm_lp

lmdone: move    $v0 $t0
        jr      $ra


swap:
        # Load list[i] into $t1
        sll     $t0, $a1, 2
        add     $t0, $a0, $t0
        lw      $t1, 0($t0)

        # Load list[j] into $t3
        sll     $t2, $a2, 2
        add     $t2, $a0, $t2
        lw      $t3, 0($t2)

        # Store $t1 in list[j] and $t3 in list[i]
        sw      $t1, 0($t2)
        sw      $t3, 0($t0)

        # Return
        jr      $ra


        .data
space:  .asciiz " "
newln:  .asciiz "\n"