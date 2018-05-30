let print = exn => {
  Printexc.to_string(exn) |> print_string;
  print_newline();
  Printexc.print_backtrace(stdout);
};