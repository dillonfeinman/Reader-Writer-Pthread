
int main(int argc, char * argv[]){
  if(argc != 4){
    cerr << "Needs 3 arguments: <N rand ints> <R readers> <W writers>." << endl;
    exit(1);
  } else {
    int n = argv[1];
    int r = argv[2];
    int w = argv[3];
    if(n < 1 || n > 1000 || r < 1 || r > 9 || w < 1 || w > 9){
      ceerr << "R and W must be between 1 and 9, inclusive, and N must be between 1 and 1000, inclusive." << endl;
      exit(2);
    } else {

    }
  }
}
