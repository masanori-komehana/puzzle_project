#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from copy import deepcopy
import solve_main as Sol



def show_puzzle(slide):
    print(slide.puzzle[0])
    print(slide.puzzle[1])
    print(slide.puzzle[2])
    print(slide.puzzle[3])

def find_blank(slide):
    board = slide.puzzle
    for i in range(len(board)):
        for j in range(len(board[i])):
            if board[i][j]==0:
                slide.x = j
                slide.y = i
    print(f"blank {(slide.x, slide.y)}")

if __name__ == '__main__':
    start = [
        [ 5,  2, 15, 13],
        [10,  0, 12,  6],
        [ 4, 14, 11,  1],
        [ 9,  7,  3,  8],
    ]
    a = Sol.Slide()
    a.puzzle = deepcopy(start)
    find_blank(a)
    show_puzzle(a)
    for i in range(4):
        print(f"move :{i}")
        a.move(i)
        show_puzzle(a)
    Sol.solve(start)

