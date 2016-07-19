local icu_utils = require "icu_utils"

describe("icu_utils", function ()
  describe("normalize", function ()
    it("handles empty string", function ()
      assert.are.equal("", icu_utils.normalize(""))
    end)

    it("lefts normalized string alone", function ()
      assert.are.equal("foobar", icu_utils.normalize("foobar"))
    end)

    it("normalizes NKD into NFC", function ()
      -- Note: strings are written as a decimal numbers of UTF-8 bytes
      local denormalized_data = "\65\109\101\204\129\108\105\101"
      local expected_data     = "\65\109\195\169\108\105\101"

      local normalized_data = icu_utils.normalize(denormalized_data)
      assert.are.equal(expected_data, normalized_data)
    end)
  end)

  describe("lower", function ()
    it("handles empty string", function ()
      assert.are.equal("", icu_utils.lower(""))
    end)

    it("handles basic ASCII string", function ()
      assert.are.equal("foobar", icu_utils.lower("FOOBAR"))
    end)

    it("handles growing string", function ()
      assert.are.equal(
        "\105\204\135", -- "U+0069 U+0307" (LATIN SMALL LETTER I and COMBINING DOT ABOVE)
        icu_utils.lower("\196\176") -- U+0130 (LATIN CAPITAL LETTER I WITH DOT ABOVE)
      )
    end)

    it("handles czech pangram", function ()
      assert.are.equal(
        "příliš žluťoučký kůň úpěl ďábelské ódy",
        icu_utils.lower("PŘÍLIŠ ŽLUŤOUČKÝ KŮŇ ÚPĚL ĎÁBELSKÉ ÓDY")
      )
    end)

    it("handles deutsch pangram", function ()
      assert.are.equal(
        "verbüss öd’ joch, kämpf qual, zwing styx!",
        icu_utils.lower("VERBÜSS ÖD’ JOCH, KÄMPF QUAL, ZWING STYX!")
      )
    end)

    it("handles french pangram", function()
      assert.are.equal(
        "dès noël où un zéphyr haï me vêt de glaçons würmiens je dîne d’exquis rôtis de bœuf au kir à l’aÿ d’âge mûr & cætera!",
        icu_utils.lower("DÈS NOËL OÙ UN ZÉPHYR HAÏ ME VÊT DE GLAÇONS WÜRMIENS JE DÎNE D’EXQUIS RÔTIS DE BŒUF AU KIR À L’AŸ D’ÂGE MÛR & CÆTERA!")
      )
    end)

    it("handles russian pangram", function()
      assert.are.equal(
        "в чащах юга жил бы цитрус? да, но фальшивый экземпляр!",
        icu_utils.lower("В ЧАЩАХ ЮГА ЖИЛ БЫ ЦИТРУС? ДА, НО ФАЛЬШИВЫЙ ЭКЗЕМПЛЯР!")
      )
    end)
  end)

  describe("upper", function ()
    it("handles empty string", function ()
      assert.are.equal("", icu_utils.upper(""))
    end)

    it("handles basic ASCII string", function ()
      assert.are.equal("FOOBAR", icu_utils.upper("foobar"))
    end)

    it("handles growing string", function ()
      assert.are.equal(
        "\83\83", -- "U+0053 U+0053" (LATIN CAPITAL LETTER S and LATIN CAPITAL LETTER S)
        icu_utils.upper("\195\159") -- U+00DF (LATIN SMALL LETTER SHARP S)
      )
    end)

    it("handles czech pangram", function ()
      assert.are.equal(
        "PŘÍLIŠ ŽLUŤOUČKÝ KŮŇ ÚPĚL ĎÁBELSKÉ ÓDY",
        icu_utils.upper("příliš žluťoučký kůň úpěl ďábelské ódy")
      )
    end)

    it("handles deutsch pangram", function ()
      assert.are.equal(
        "VERBÜSS ÖD’ JOCH, KÄMPF QUAL, ZWING STYX!",
        icu_utils.upper("verbüß öd’ joch, kämpf qual, zwing styx!")
      )
    end)

    it("handles french pangram", function()
      assert.are.equal(
        "DÈS NOËL OÙ UN ZÉPHYR HAÏ ME VÊT DE GLAÇONS WÜRMIENS JE DÎNE D’EXQUIS RÔTIS DE BŒUF AU KIR À L’AŸ D’ÂGE MÛR & CÆTERA!",
        icu_utils.upper("dès noël où un zéphyr haï me vêt de glaçons würmiens je dîne d’exquis rôtis de bœuf au kir à l’aÿ d’âge mûr & cætera!")
      )
    end)

    it("handles russian pangram", function()
      assert.are.equal(
        "В ЧАЩАХ ЮГА ЖИЛ БЫ ЦИТРУС? ДА, НО ФАЛЬШИВЫЙ ЭКЗЕМПЛЯР!",
        icu_utils.upper("в чащах юга жил бы цитрус? да, но фальшивый экземпляр!")
      )
    end)
  end)

  describe("capitalize", function ()
    it("handles empty string", function ()
      assert.are.equal("", icu_utils.capitalize(""))
    end)

    it("handles basic ASCII word", function ()
      assert.are.equal("Foobar", icu_utils.capitalize("foobar"))
    end)

    it("handles basic ASCII pharse", function ()
      assert.are.equal("Lorem Ipsum", icu_utils.capitalize("lorem ipsum"))
    end)

    it("handles growing string", function ()
      assert.are.equal(
        "Ss",
        icu_utils.capitalize("\195\159") -- U+00DF (LATIN SMALL LETTER SHARP S)
      )
    end)

    it("handles czech pangram", function ()
      assert.are.equal(
        "Příliš Žluťoučký Kůň Úpěl Ďábelské Ódy",
        icu_utils.capitalize("příliš žluťoučký kůň úpěl ďábelské ódy")
      )
    end)
  end)

  describe("latinize", function ()
    it("handles empty string", function ()
      assert.are.equal("", icu_utils.latinize(""))
    end)

    it("lefts ascii string alone", function ()
      assert.are.equal("foobar", icu_utils.latinize("foobar"))
    end)

    -- Note: Result of french pangram is actually longer then the original string
    -- so it is good to test memory reallocation.
    it("handles longer result than the original", function ()
      assert.are.equal(
        "oeoeoeoe",
        icu_utils.latinize("œœœœ") -- U+0152
      )
    end)

    it("handles czech pangram", function ()
      assert.are.equal(
        "prilis zlutoucky kun upel dabelske ody",
        icu_utils.latinize("příliš žluťoučký kůň úpěl ďábelské ódy")
      )
    end)

    it("handles deutsch pangram", function ()
      assert.are.equal(
        "Verbuss od' Joch, kampf Qual, zwing Styx!",
        icu_utils.latinize("Verbüß öd’ Joch, kämpf Qual, zwing Styx!")
      )
    end)

    it("handles french pangram", function()
      assert.are.equal(
        "Des Noel ou un zephyr hai me vet de glacons wurmiens je dine d'exquis rotis de boeuf au kir a l'ay d'age mur & caetera!",
        icu_utils.latinize("Dès Noël où un zéphyr haï me vêt de glaçons würmiens je dîne d’exquis rôtis de bœuf au kir à l’aÿ d’âge mûr & cætera!")
      )
    end)

    it("handles russian pangram", function()
      assert.are.equal(
        "V casah uga zil by citrus? Da, no falʹsivyj ekzemplar!",
        icu_utils.latinize("В чащах юга жил бы цитрус? Да, но фальшивый экземпляр!")
      )
    end)
  end)
end)
