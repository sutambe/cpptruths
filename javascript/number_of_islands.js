function mark_recursive(i, j, grid, ROWS, COLS) {
  if (grid[i][j] === '1') {
      grid[i][j] = 'v';
      if (i-1 >= 0) {
          mark_recursive(i-1, j, grid, ROWS, COLS);
      }
      if (j+1 < COLS) {
          mark_recursive(i, j+1, grid, ROWS, COLS);
      }
      if (i+1 < ROWS) {
          mark_recursive(i+1, j, grid, ROWS, COLS);
      }
      if (j-1 >= 0) {
          mark_recursive(i, j-1, grid, ROWS, COLS);
      }
  }    
}

/**
 * @param {character[][]} grid
 * @return {number}
 */
var numIslands = function(grid) {
    var islands = 0;
    var ROWS = grid.length;
    if (grid.length == 0) return islands;
    if (grid[0].length == 0) return islands;
    var COLS = grid[0].length;
    for (var i = 0;i < ROWS; i++) {
        for (var j = 0;j < COLS; j++) {
          if (grid[i][j] === '1') {
              mark_recursive(i, j, grid, ROWS, COLS);
              islands += 1;
          }
        }
    }
        
    return islands;
};
