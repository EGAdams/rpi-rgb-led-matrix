After changing the "embed_user_input" column to "vector", I get this error now:
```error
Exception has occurred: ArrowNotImplementedError       (note: full exception trace is shown but execution is paused at: _run_module_as_main)
Unsupported cast from fixed_size_list<item: float>[512] to list using function cast_list
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/pyarrow/error.pxi", line 121, in pyarrow.lib.check_status
    raise ArrowNotImplementedError(message)
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/pyarrow/error.pxi", line 144, in pyarrow.lib.pyarrow_internal_check_status
    return check_status(status)
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/pyarrow/_compute.pyx", line 367, in pyarrow._compute.Function.call
    result = GetResultValue(
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/pyarrow/_compute.pyx", line 572, in pyarrow._compute.call_function
    return func.call(args, options=options, memory_pool=memory_pool,
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/pyarrow/compute.py", line 403, in cast
    return call_function("cast", [arr], options, memory_pool)
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/pyarrow/table.pxi", line 552, in pyarrow.lib.ChunkedArray.cast
    return _pc().cast(self, target_type, safe=safe, options=options)
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/pyarrow/array.pxi", line 354, in pyarrow.lib.asarray
    values = values.cast(type)
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/pyarrow/table.pxi", line 1450, in pyarrow.lib._sanitize_arrays
    item = asarray(item, type=schema[i].type)
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/pyarrow/table.pxi", line 3862, in pyarrow.lib.Table.from_arrays
    converted_arrays = _sanitize_arrays(arrays, names, schema, metadata,
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/lancedb/table.py", line 896, in _sanitize_schema
    return pa.Table.from_arrays(
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/lancedb/table.py", line 66, in _sanitize_data
    data = _sanitize_schema(
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/site-packages/lancedb/table.py", line 547, in add
    data = _sanitize_data(
  File "/home/adamsl/linuxBash/pinecone_chat_dave_s/chat_with_lancedb.py", line 213, in __init__
    self.table.add( dataframe )
  File "/home/adamsl/linuxBash/pinecone_chat_dave_s/chat_with_lancedb.py", line 231, in <module>
    lanceTable = LanceTable()
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/runpy.py", line 86, in _run_code
    exec(code, run_globals)
  File "/home/adamsl/.pyenv/versions/3.10.6/lib/python3.10/runpy.py", line 196, in _run_module_as_main (Current frame)
    return _run_code(code, main_globals, None,
pyarrow.lib.ArrowNotImplementedError: Unsupported cast from fixed_size_list<item: float>[512] to list using function cast_list
```