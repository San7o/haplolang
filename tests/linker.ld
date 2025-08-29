SECTIONS
{
  /* Create a real output section we can insert */
  .utest_records :
  {
    __start_utest_records = .;
    KEEP(*(.utest_records))
    __stop_utest_records = .;
  }
}
INSERT AFTER .data;
