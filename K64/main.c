#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/fields.h"
#include "libfdr/jrb.h"
#include "GraphAPI/graph_jrb.h"
char *findName(JRB name_ID, int i);
char *ficName(char*s);
void readFile(Graph g,char *filename,JRB name_ID);


void option2(Graph g){
  int path[1000], ID1, ID2;
  char dinh1[30], dinh2[30];
  printf("Nhap dinh 1: ");
  rewind(stdin);
  gets(dinh1);
  JRB node;
  jrb_traverse(node, g.vertices)
  {
    if (strcmp(dinh1, jval_s(node->val)) == 0)
      ID1 = node->key.i;
  }
  printf("Nhap dinh 2: ");
  rewind(stdin);
  gets(dinh2);
  jrb_traverse(node, g.vertices)
  {
    if (strcmp(dinh2, jval_s(node->val)) == 0)
      ID2 = node->key.i;
  }
  int value;
  double tien = shortestPath(g, ID1, ID2, path, &value);
  if (value == 0)
    printf("Khong co duong di\n");
  else
  {
    printf("So tien la: %.1f\n", tien);
    for (int i = 0; i < value; i++)
    {
      printf("%s", getVertex(g, path[i]));
      if (i < value - 1)
        printf(" - ");
      else
        printf("\n");
    }
  }
}

int main(){
  
  Graph g = createGraph();
  JRB name_ID= make_jrb();
  readFile(g, "busdata.txt",name_ID);

  // Graph g1 = createGraph();
  int selection ;
  while (selection!=2)
  {
    printf("--------------------Menu-----------------\n");
    printf("1. In ra cac diem bus \n");
    printf("2. Tim duong di ngan nhat giua 2 diem bus \n");
    printf("--------------------End-----------------\n");
    printf("---> Chon:\n");
    scanf("%d", &selection);
    switch (selection)
    {
    case 1:
      /* code */
      {
        JRB node1, node2;
        jrb_traverse(node1, g.vertices)
        {
          int id1 = jval_i(node1->key);
          jrb_traverse(node2, g.vertices)
          {
            int id2 = jval_i(node2->key);
            weight w =getEdgeValue(g, id1, id2);
            if (hasEdge(g, node1->key.i, node2->key.i))
            {
              printf("%s %s %d\n", findName(name_ID, id1), findName(name_ID, id2), w->val);
            }
          }
        }
      }
      break;
    case 2:
     option2(g);
    
    default:
      break;
    }
    /* code */
  }
  
  return 0;
}

char *fixName(char *s)
{
  int n = strlen(s);
  char *s1 = (char *)malloc(sizeof(char) * n);
  for (int i = 0; i < n - 3; i++)
  {
    s1[i] = s[i + 1];
  }
  s1[n - 3] = '\0';
  return s1;
}
char *findName(JRB name_ID, int i)
{
  JRB node;
  jrb_traverse(node, name_ID)
  {
    if (node->val.i == i)
      return jval_s(node->key);
  }
}
void readFile(Graph g, char *filename, JRB name_ID)
{
  IS is = new_inputstruct(filename);
  if (is == NULL)
  {
    printf("File fail!\n");
    exit(1);
  }
  int i = 0;
  const char s[2] = "=";
  char *nameID = (char *)malloc(sizeof(char) * 10);
  char *name = (char *)malloc(sizeof(char) * 30);

  while (get_line(is) > 0)
  {
    char *check = strchr(is->text1, '=');
    if (check != NULL)
    {
      nameID = strtok(is->text1, s);
      name = strtok(NULL, s);
      // printf("%s \n",fixName(name));
      // printf("%d truoc",strlen(nameID));

      JRB find = jrb_find_str(name_ID, nameID);
      if (find == NULL)
      {
        i++;
        jrb_insert_str(name_ID, strdup(nameID), new_jval_i(i));
        addVertex(g, i, fixName(name));
      }
    }
    else
    {
      char place1[8], place2[8];
      strcpy(place1, is->fields[0]);
      strcpy(place2, is->fields[1]);
      // printf("%d sau",strlen(place1));
      // printf("%s\n",place1);
      // printf("%s\n",place2);

      JRB find1 = jrb_find_str(name_ID, place1);
      JRB find2 = jrb_find_str(name_ID, place2);
      weight w = new_weight();
      // printf("hear %s",place1);
      for (int i = 2; i <= 3; i++)
      {
        if (is->fields[i] != NULL)
        {

          int b = atoi(is->fields[i]);
          if (b != 0)
          {
            w->arrBus[w->nbus] = b;
            w->nbus++;
          }
        }
      }
      // printf("%d\n",find1->val.i);
      // printf(" find 2%d\n",find2->val.i);

      // for(int i=0;i<w->nbus;i++){
      //   printf("%d\n",w->arrBus[i]);
      // }
      addEdge(g, find1->val.i, find2->val.i, w);
    }
  }
}