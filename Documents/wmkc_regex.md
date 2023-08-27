# Visual Studio Code Regex

### WMKC naming rule matching and replacement.

- Match
```regex
wmkcErr_return\(error, wmkcErr_(\w+),([\n ,]+)"(\w+): ([\w\d .,\(\)*->]+)"
```

- Replace
```regex
wmkcErr_func_return(error, wmkcErr_$1, "$3",$2"$4"
```
