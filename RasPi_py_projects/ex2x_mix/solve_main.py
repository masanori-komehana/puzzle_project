
# -*- coding: utf-8 -*-

import random
import numpy as np      
import copy
from pprint import pprint

import Astar as As

class Slide():
    """
    0　が　操作マス
    左上を原点とする
    横に　ｘ
    縦に　ｙ  
    トスル

    """

    def __init__(self):
        self.puzzle = [[1,2,3,4],
                  [5,6,7,8],
                  [9,10,11,12],
                  [13,14,15,0]]
        self.x = 3   #操作マス　0 の場所
        self.y = 3

        self.fixed = np.zeros((4,4))
        self.route = [[self.x,self.y]]

    def route_clear(self):
        self.route = [[self.x,self.y]]

    def shuffle(self,num):
        for i in range(num):
            j=0
            while True:
                j = random.randint(0,3)
                break
            self.move(j)

    def move(self,direction):
        """
           0
        3     1
           2

        動くのに失敗したら　-1
        動いたら　　　　　　　0
        """
        if direction == 0:
            if self.y != 0:
                if self.fixed[self.y-1][self.x] == 0:
                    self.puzzle[self.y][self.x] = self.puzzle[self.y-1][self.x]
                    self.y = self.y - 1
                    self.puzzle[self.y][self.x] = 0
                    self.route.append([self.x,self.y])
                    return 0
                else:
                    return -1
        if direction == 1:
            if self.x != 3:
                if self.fixed[self.y][self.x+1] == 0:
                    self.puzzle[self.y][self.x] = self.puzzle[self.y][self.x+1]
                    self.x = self.x + 1
                    self.puzzle[self.y][self.x] = 0
                    self.route.append([self.x,self.y])
                    return 0
                else:
                    return -1
        if direction == 2:
            if self.y != 3:
                if self.fixed[self.y+1][self.x] == 0:
                    self.puzzle[self.y][self.x] = self.puzzle[self.y+1][self.x]
                    self.y = self.y + 1
                    self.puzzle[self.y][self.x] = 0
                    self.route.append([self.x,self.y])
                    return 0
                else:
                    return -1
        if direction == 3:
            if self.x != 0:
                if self.fixed[self.y][self.x-1] == 0:
                    self.puzzle[self.y][self.x] = self.puzzle[self.y][self.x-1]
                    self.x = self.x - 1
                    self.puzzle[self.y][self.x] = 0
                    self.route.append([self.x,self.y])
                    return 0
                else:
                    return -1
        return -1


    def move_0(self,x,y):
        """
       操作マス("0"マス) を　x,y に動かす       

       先にルートを考え、fixに引っかからないとわかってから、移動を行います。
       これはある種の迷路です。
       別の関数で　迷路を解くための　A*　アルゴリズムを実装し、それを利用して解きましょう

        """

        hoge = As.Astar(x,y,self.x,self.y,self.fixed)
        result = hoge.explore()
        route = []
        if result == 0:
           route = hoge.route
        else:
            return -1


        for i in range(len(route)-1):
            if route[i][0] <  route[i+1][0]:
                self.move(1)
            elif route[i+1][0] <  route[i][0]:
                self.move(3)
            elif route[i+1][1] <  route[i][1]:
                self.move(0)
            elif route[i+1][1] >  route[i][1]:
                self.move(2)

        if self.x !=x or self.y != y:
            return -1
        else:
            return 0

    def move_any(self,position,direction):
        x=position[0]
        y=position[1]
        """
        任意の　"数"(x,y) を　direction の方向に動かす   
           0
        3     1
           2

        動くのに失敗したら　-1
        動いたら　　　　　　　0
        """
        if direction == 0:    #上に移動させる  操作マスを上に付ける
            self.fixed[y][x] = 1
            hoge = self.move_0(x,y-1)
            self.fixed[y][x] = 0
            if hoge == -1:
                return -1
            else:
                self.move_0(x,y)           
                return 0

        elif direction == 2:  # 下に移動させる
            self.fixed[y][x] = 1
            hoge = self.move_0(x,y+1)
            self.fixed[y][x] = 0
            if hoge == -1:
                return -1
            else:
                self.move_0(x,y)           
                return 0

        elif direction == 1:  # 右に移動させる
            self.fixed[y][x] = 1
            hoge = self.move_0(x+1,y)
            self.fixed[y][x] = 0
            if hoge == -1:
                return -1
            else:
                self.move_0(x,y)           
                return 0

        elif direction == 3:  # 左に移動させる
            self.fixed[y][x] = 1
            hoge = self.move_0(x-1,y)
            self.fixed[y][x] = 0
            if hoge == -1:
                return -1
            else:
                self.move_0(x,y)           
                return 0    

    def find_position(self,num):
        for i in range(4):
            for j in range(4):
                if self.puzzle[i][j] == num:
                    return (j,i)

    def move_x(self,number,position):
        target_x = position[0]
        target_y = position[1]
        """
        def move_any(self,position,direction):
        任意の　"数"(x,y) を　direction の方向に動かす   
           0
        3     1
           2

        動くのに失敗したら　-1
        動いたら　　　　　　　0
        """
        position2 = self.find_position(number)
        now_x = position2[0]
        now_y = position2[1]
        """
        Astar アルゴリズムで　number　の　ルート　見つけ
        ルートに従い、move_anyで動かし
        但し、道幅が広くないと成立しないので、move_anyが失敗する可能性がある
        －＞Astar , fixする順序で対処
        """        
        hoge = As.Astar(target_x,target_y,now_x,now_y,self.fixed)
        result = hoge.explore()
        route = []
        if result == 0:
            route = hoge.route
        else:
            return -1

        for i in range(len(route)-1):
            position2 = self.find_position(number)
            now_x = position2[0]
            now_y = position2[1]
            if route[i][0] <  route[i+1][0]:
                result = self.move_any((now_x,now_y),1)
                if result == -1:
                    return -1

            elif route[i+1][0] <  route[i][0]:
                result = self.move_any((now_x,now_y),3)
                if result == -1:
                    return -1

            elif route[i+1][1] <  route[i][1]:
                result = self.move_any((now_x,now_y),0)
                if result == -1:
                    return -1

            elif route[i+1][1] >  route[i][1]:
                result = self.move_any((now_x,now_y),2)
                if result == -1:
                    return -1

        position2 = self.find_position(number)
        now_x = position2[0]
        now_y = position2[1]
        if target_x != now_x or target_y != now_y:
            return -1
        else:
            return 0
    def exchange_row(self):
        """
        4 3
        x 0
        y z
        で入れ替える
        """
        self.move(0)
        self.move(3)
        """
        0 4
        x 3
        y z       
        """
        self.move(2)
        self.move(2)
        self.move(1)
        """
        x 4
        y 3
        z 0        
        """
        self.move(0)
        """
        x 4
        y 0
        z 3       
        """
        self.move(3)
        self.move(0)
        self.move(1)
        """
        4 0
        x y
        z 3       
        """

    def exchange_line(self):
        """
        13 0 y
        9  x z
        """
        self.move(3)
        self.move(2)
        """
        9 13 y
        0  x z
        """
        self.move(1)
        self.move(1)
        self.move(0)
        """
        9 13 0
        x y z
        """
        self.move(3)
        """
        9 0 13
        x y z
        """
        self.move(2)
        self.move(3)
        self.move(0)
        """
        0 y 13
        9 x z
        """

def route_test(slide,route):
    if route == []:
        return -1
    else:
        for i in range(len(route)-1):
            if route[i][0] <  route[i+1][0]:
                slide.move(1)
            elif route[i+1][0] <  route[i][0]:
                slide.move(3)
            elif route[i+1][1] <  route[i][1]:
                slide.move(0)
            elif route[i+1][1] >  route[i][1]:
                slide.move(2)
    return slide

def make_route_serial_lst(route):
    lst = []
    if route == []:
        return -1
    else:
        for i in range(len(route)-1):
            c = ''
            if route[i][0] <  route[i+1][0]:
                c = 'a'
            elif route[i+1][0] <  route[i][0]:
                c = 'd'
            elif route[i+1][1] <  route[i][1]:
                c = 's'
            elif route[i+1][1] >  route[i][1]:
                c = 'w'
            if c != '': lst.append(c)
        # pprint(lst)
    return lst

def show_puzzle(slide):
    print(slide.puzzle[0])
    print(slide.puzzle[1])
    print(slide.puzzle[2])
    print(slide.puzzle[3])

def solve(board):
    slide = Slide()
    slide.puzzle = board
    for i in range(len(board)):
        for j in range(len(board[i])):
            if board[i][j]==0:
                slide.x = j
                slide.y = i
                slide.route_clear()

    #1
    slide.move_x(1,(0,0))
    slide.fixed[0][0] =1
    #2
    slide.move_x(2,(1,0))
    slide.fixed[0][1] =1
    #3,4
    slide.move_x(4,(2,0))
    slide.fixed[0][2] =1

    if slide.x == 3 and slide.y == 0 and slide.puzzle[1][3] == 3:
        slide.move(2)
    if slide.puzzle[0][3] == 3:
        slide.fixed[0][2] = 0
        slide.move_0(3,1)
        slide.exchange_row()
        print("errored 3-4")

    slide.move_x(3,(2,1))
    slide.fixed[1][2] = 1
    slide.move_0(3,0)
    slide.fixed[1][2] = 0
    slide.fixed[0][2] = 0
    slide.move(3)
    slide.move(2)

    slide.fixed[0][2] = 1
    slide.fixed[0][3] = 1


    #5
    slide.move_x(5,(0,1))
    slide.fixed[1][0] =1
    #9,13
    slide.move_x(9,(0,3))
    slide.fixed[3][0] =1

    if slide.x == 0 and slide.y == 2 and slide.puzzle[2][1] == 13:
        slide.move(1)
    if slide.puzzle[2][0] == 13:
        slide.fixed[3][0] = 0
        slide.move_0(1,2)
        slide.exchange_line()
        slide.fixed[3][0] = 1

    slide.move_x(13,(1,3))
    slide.fixed[3][1] = 1
    slide.move_0(0,2)
    slide.fixed[3][1] = 0
    slide.fixed[3][0] = 0
    slide.move(2)
    slide.move(1)

    slide.fixed[2][0] = 1
    slide.fixed[3][0] = 1


    #6
    slide.move_x(6,(1,1))
    slide.fixed[1][1] =1

    #7,8
    slide.move_x(8,(2,1))
    slide.fixed[1][2] =1
    if slide.x == 3 and slide.y == 1 and slide.puzzle[2][3] == 7:
        slide.move(2)
    if slide.puzzle[1][3] == 7:
        slide.fixed[1][2] = 0
        slide.move_0(3,2)
        slide.exchange_row()

    slide.move_x(7,(2,2))
    slide.fixed[2][2] = 1
    slide.move_0(3,1)
    slide.fixed[2][2] = 0
    slide.fixed[1][2] = 0
    slide.move(3)
    slide.move(2)

    slide.fixed[1][3] = 1
    slide.fixed[1][2] = 1

    #10,14
    result = slide.move_x(10,(1,3))

    slide.fixed[3][1] =1
    if slide.x == 1 and slide.y == 2 and slide.puzzle[2][2] == 14:
        slide.move(1)
    if slide.puzzle[2][1] == 14:
        slide.fixed[3][1] = 0
        slide.move_0(2,2)
        slide.exchange_line()
        slide.fixed[3][1] = 1


    slide.move_x(14,(2,3))
    slide.fixed[3][2] = 1
    slide.move_0(1,2)
    slide.fixed[3][2] = 0
    slide.fixed[3][1] = 0
    slide.move(2)
    slide.move(1)

    slide.fixed[2][1] = 1
    slide.fixed[3][1] = 1


    # これで行けるかと思ったが　、ちょっと違った
    slide.move_0(3,3)


    if slide.puzzle[3][2] == 11:
        #反時計回り一周
        slide.move(0)
        slide.move(3)
        slide.move(2)
        slide.move(1)
    elif slide.puzzle[3][2] == 12:
        #時計回り一周
        slide.move(3)
        slide.move(0)
        slide.move(1)
        slide.move(2)

    print(len(slide.route))
    print(slide.puzzle[0])
    print(slide.puzzle[1])
    print(slide.puzzle[2])
    print(slide.puzzle[3])

    return slide


if __name__ == '__main__' :
    hoge = Slide()
    hoge.shuffle(600)
    hoge.route_clear()
    print(hoge.puzzle[0])
    print(hoge.puzzle[1])
    print(hoge.puzzle[2])
    print(hoge.puzzle[3])

    #test用
    hoge2 = Slide()
    hoge2.puzzle = copy.deepcopy(hoge.puzzle)
    hoge2.x = hoge.x
    hoge2.y = hoge.y


    #1
    hoge.move_x(1,(0,0))
    hoge.fixed[0][0] =1
    #2
    hoge.move_x(2,(1,0))
    hoge.fixed[0][1] =1
    #3,4
    hoge.move_x(4,(2,0))
    hoge.fixed[0][2] =1

    if hoge.x == 3 and hoge.y == 0 and hoge.puzzle[1][3] == 3:
        hoge.move(2)
    if hoge.puzzle[0][3] == 3:
        hoge.fixed[0][2] = 0
        hoge.move_0(3,1)
        hoge.exchange_row()
        print("errored 3-4")

    hoge.move_x(3,(2,1))
    hoge.fixed[1][2] = 1
    hoge.move_0(3,0)
    hoge.fixed[1][2] = 0
    hoge.fixed[0][2] = 0
    hoge.move(3)
    hoge.move(2)

    hoge.fixed[0][2] = 1
    hoge.fixed[0][3] = 1


    #5
    hoge.move_x(5,(0,1))
    hoge.fixed[1][0] =1
    #9,13
    hoge.move_x(9,(0,3))
    hoge.fixed[3][0] =1

    if hoge.x == 0 and hoge.y == 2 and hoge.puzzle[2][1] == 13:
        hoge.move(1)
    if hoge.puzzle[2][0] == 13:
        hoge.fixed[3][0] = 0
        hoge.move_0(1,2)
        hoge.exchange_line()
        print("error 9-13")
        hoge.fixed[3][0] = 1

    hoge.move_x(13,(1,3))
    hoge.fixed[3][1] = 1
    hoge.move_0(0,2)
    hoge.fixed[3][1] = 0
    hoge.fixed[3][0] = 0
    hoge.move(2)
    hoge.move(1)

    hoge.fixed[2][0] = 1
    hoge.fixed[3][0] = 1

    #6
    hoge.move_x(6,(1,1))
    hoge.fixed[1][1] =1

    #7,8
    hoge.move_x(8,(2,1))
    hoge.fixed[1][2] =1
    if hoge.x == 3 and hoge.y == 1 and hoge.puzzle[2][3] == 7:
        hoge.move(2)
    if hoge.puzzle[1][3] == 7:
        hoge.fixed[1][2] = 0
        hoge.move_0(3,2)
        hoge.exchange_row()
        print("error 7-8")

    hoge.move_x(7,(2,2))
    hoge.fixed[2][2] = 1
    hoge.move_0(3,1)
    hoge.fixed[2][2] = 0
    hoge.fixed[1][2] = 0
    hoge.move(3)
    hoge.move(2)

    hoge.fixed[1][3] = 1
    hoge.fixed[1][2] = 1

    #6マスなので　もう　ゲーム木探索でも解けるのでは？
    #10,14
    result = hoge.move_x(10,(1,3))
    print(str(result)+"result")

    hoge.fixed[3][1] =1
    if hoge.x == 1 and hoge.y == 2 and hoge.puzzle[2][2] == 14:
        hoge.move(1)
    if hoge.puzzle[2][1] == 14:
        hoge.fixed[3][1] = 0
        hoge.move_0(2,2)
        hoge.exchange_line()
        print("error10-14")
        hoge.fixed[3][1] = 1


    hoge.move_x(14,(2,3))
    hoge.fixed[3][2] = 1
    hoge.move_0(1,2)
    hoge.fixed[3][2] = 0
    hoge.fixed[3][1] = 0
    hoge.move(2)
    hoge.move(1)

    hoge.fixed[2][1] = 1
    hoge.fixed[3][1] = 1


    # これで行けるかと思ったが　、ちょっと違った
    hoge.move_0(3,3)

    print("a")
    print(hoge.puzzle[0])
    print(hoge.puzzle[1])
    print(hoge.puzzle[2])
    print(hoge.puzzle[3])
    print(hoge.fixed[0])
    print(hoge.fixed[1])
    print(hoge.fixed[2])
    print(hoge.fixed[3])

    if hoge.puzzle[3][2] == 11:
        #反時計回り一周
        hoge.move(0)
        hoge.move(3)
        hoge.move(2)
        hoge.move(1)
    elif hoge.puzzle[3][2] == 12:
        #時計回り一周
        hoge.move(3)
        hoge.move(0)
        hoge.move(1)
        hoge.move(2)

    print(hoge.puzzle[0])
    print(hoge.puzzle[1])
    print(hoge.puzzle[2])
    print(hoge.puzzle[3])


    print(len(hoge.route))

    hoge2 = route_test(hoge2,hoge.route)
    if hoge2 == -1:
        print("error")
    else:
        print(hoge2.puzzle[0])
        print(hoge2.puzzle[1])
        print(hoge2.puzzle[2])
        print(hoge2.puzzle[3])




