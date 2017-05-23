return {
  init = function(self, score)
    print('score init')
    clearWorld()
    print('after clear')
    if score[1] >= 1 then
      print('Player 1 wins')
    elseif score[2] >= 1 then
      print('Player 2 wins')
    end
  end
}
