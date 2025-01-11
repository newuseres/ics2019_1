int init_monitor(int, char *[]);
void ui_mainloop(int);
void TEST_expr();
int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
  //TEST_expr();
  /* Receive commands from user. */
  Log("wh");
  ui_mainloop(is_batch_mode);

  return 0;
}
