[프로젝트 구성에 관한 간단한 설명]

SimpleIndex와 SimpleIndex header 폴더에 있는 .h , .cpp 화일은 이미 Appendix G에서 이미 구현된 부분입니다.
따라서, 컴파일시에 꼭 필요하지만, 새로 추가되는 부분에 대한 명확한 분리를 위해서 따라 묶어 둔 겁니다.


새로 추가된 화일 
tetbtree.cpp     b-tree 를 테스트 하기 위한 main함수 
btnode.h         b-tree의 한 노드에 관련된 변수와 함수로 구성  ( 교재의 btnode.h와 btnode.tc를 하나로 합쳤음 )
btree.h          b-tree 전체에 대한 변수와 함수로 구성 ( 교재의 btee.h와 bree.tc를 하나로 합쳤음 )

 