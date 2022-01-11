import numpy as np      #   Numpyライブラリ
import random
import copy

class Node():
    def __init__(self,x,y,cost,parent,num):
        self.x = x
        self.y = y
        self.state = 1   # 0:none 1:open 2:closed
        self.score = 0
        self.cost = cost
        self.parent = parent
        self.expect_cost =  0
        self.num = num
        self.calculated = 0


    def close(self):
        self.state = 2




class Astar():
    def __init__(self,g_x,g_y,s_x,s_y,obstacle):
        self.width = obstacle.shape[1]
        self.height = obstacle.shape[0]
        self.g_x = g_x
        self.g_y = g_y
        self.s_x = s_x
        self.s_y = s_y
        self.x = s_x
        self.y = s_y
        self.obstacle_list = copy.deepcopy(obstacle)
        self.maked_list = []
        self.num = 0

        start = Node(s_x,s_y,0,-1,self.num)
        self.Node_list = [start]
        self.num = self.num + 1
        self.now = start           #現在のノード
        self.route = []
        self.goal = -1            #gaal の　ノード
        self.finished = 0         #goal したかどうか

        if g_x == s_x and g_y == s_y:
            self.finished == 1
            self.goal = start
            self.route = [[s_x,s_y]]


    def open(self):
        self.now.close()
        #周りをopen
        """
        壁・障害　が有るときはopen できない　－＞obstacle_list
        既に作っていないか？－＞maked_list 
        """
        cost = self.now.cost
        parent = self.now.num
        if self.x!=0:
            if self.maked_list.count([self.x-1,self.y]) == 0 and  self.obstacle_list[self.y][self.x-1] == 0 :
                 self.Node_list.append(Node(self.x-1,self.y,cost+1,parent,self.num))
                 self.num = self.num + 1
                 self.maked_list.append([self.x-1,self.y])
        if self.x!=self.width-1:
            if self.maked_list.count([self.x+1,self.y]) == 0 and  self.obstacle_list[self.y][self.x+1] == 0 :
                 self.Node_list.append(Node(self.x+1,self.y,cost+1,parent,self.num))
                 self.num = self.num + 1
                 self.maked_list.append([self.x+1,self.y])
        if self.y!=0:
            if self.maked_list.count([self.x,self.y-1]) == 0 and  self.obstacle_list[self.y-1][self.x] == 0 :
                 self.Node_list.append(Node(self.x,self.y-1,cost+1,parent,self.num))
                 self.num = self.num + 1
                 self.maked_list.append([self.x,self.y-1])
        if self.y!=self.height-1:
            if self.maked_list.count([self.x,self.y+1]) == 0 and  self.obstacle_list[self.y+1][self.x] == 0 :
                 self.Node_list.append(Node(self.x,self.y+1,cost+1,parent,self.num))
                 self.num = self.num + 1
                 self.maked_list.append([self.x,self.y+1])

        """
        #デバッグ
        print("test")
        for i in self.Node_list:
            print(i.state)
        """ 

        #open しているものを計算
        for i in self.Node_list:
            if i.state == 1 and i.calculated == 0:
                i.calculated = 1
                i.expect_cost = abs(i.x - self.g_x)+abs(i.y-self.g_y)
                i.score = i.cost + i.expect_cost

        #open しているもののうち、スコアの小さいものをリストにまとめる
        min_cost  = 100
        min_cost_list = []
        for i in self.Node_list:
            if i.state == 1:
                if i.cost < min_cost:
                    min_cost = i.cost
                    min_cost_list = [i]
                elif i.cost == min_cost:
                    min_cost_list.append(i)

        if min_cost_list != []:
            self.now = min_cost_list[random.randint(0,len(min_cost_list)-1)]
            self.x = self.now.x
            self.y = self.now.y
        else:
            print("none min")
            return -1

        if self.now.x == self.g_x and self.now.y == self.g_y:
            return 1
        else:
            return 0



    def explore(self):
        """
        0 :goal
        -1:goal できない
        """
        if self.finished == 1:
            return 0
        else:
            while True:
                hoge = self.open()
                if hoge == 1:
                    #print("goal!")
                    self.goal = self.now
                    self.finished = 1
                    self.Route()
                    return 0
                elif hoge == -1:
                    return -1


    def Route(self):
        if self.finished == 1:
            while True:
                self.route.append((self.now.x,self.now.y))
                if self.now.parent == -1:
                    break
                else:
                    self.now = self.Node_list[self.now.parent]

            self.route.reverse()
            #print(self.route)

    def Express(self):
        if self.finished == 1:
            if self.route ==[]:
                print("not goaled")
            else:
                graph    =  self.obstacle_list
                for i in self.route:
                    graph[i[1]][i[0]] = 2

                print(graph)




if __name__ == '__main__' :
    width = 5
    height = 5
    obstacle    =np.zeros((height,width))
    """
    obstacle[2][1] = 1
    obstacle[2][2] = 1
    obstacle[1][2] = 1
    obstacle[3][2] = 1
    """
    obstacle[1][0] = 1
    print(obstacle)
    g_x = 0
    g_y = 2
    s_x = 0
    s_y = 0

    hoge = Astar(g_x,g_y,s_x,s_y,obstacle)  
    result = hoge.explore()
    if result == 0:
        hoge.Express()
