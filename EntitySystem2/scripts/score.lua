return {
  init = function(self, score)
    print('score init')
    clearWorld()
    print('after clear')
    text = createEntity({
      transform = { x = 0, y = 0}
    })
    if score[1] >= 1 then
      text:attach(Components.Text, {
        text = 'Player 1 wins'
      })
      print('Player 1 wins')
    elseif score[2] >= 1 then
      text:attach(Components.Text, {
        text = 'Player 2 wins'
      })
      print('Player 2 wins')
    end

  end
}
