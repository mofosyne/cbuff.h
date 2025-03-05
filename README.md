# cbuff.h Circular Byte Buffer For Embedded Applications

<versionBadge>![Version 0.1.1](https://img.shields.io/badge/version-0.1.1-blue.svg)</versionBadge>
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CI/CD Status Badge](https://github.com/mofosyne/cbuff.h/actions/workflows/ci.yml/badge.svg)](https://github.com/mofosyne/cbuff.h/actions)

Malloc free, minimum overhead implementation of a circular buffer.

Static inlined handlers for speed and ease of usage in various projects.

This is based on harshkn's circular buffer: https://gist.github.com/harshkn/909546

## Design Considerations Applied

* Favor index approach over pointer approach
    - This is because I previously written a circular buffer implementation in github gist as [Pointer Approach](https://gist.github.com/mofosyne/d7a4a8d6a567133561c18aaddfd82e6f) vs [Index Approach](https://gist.github.com/mofosyne/82020d5c0e1e11af0eb9b05c73734956)
    - According to GodBolt compiler explorer, index approach has less lines of code over pointer approach. [Pointer Approach](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIAKwA7KSuADJ4DJgAcj4ARpjEIIHBAA6oCoRODB7evgGp6ZkC4ZExLPGJybaY9o4CQgRMxAQ5Pn5mGtW1WQ1NBCXRcQlJwQqNza15Hbbj/RGD5cPJAJS2qF7EyOwcAPQ7ANTmAMz7bnibXgbE%2BxYAngSYN15UVAn7AGIk%2B9iL6Fjo%2BwAgikUrQ8KI6gwFPsIMpUBEHtcLEwlOhliYNIC9kCvAQECQQDdiHhDPsANIILxeGEAKUut32HQ6yxhsWJhgA1pSvAA6NAsFkQFiofgKW6RAACwBYTDofNQAoxWIOACVMCjnPsALJiWioZD7KjETCYUj7FgRPAsHz7VAANwSCHVAKtoMwbEETEhtqo%2ByY%2B2Q52Qlya%2B1iz1exB5Sux%2B32vUcBoiYMiAIQhnQ9GI0P41wUKRNAIz%2B3VSh9%2By8CiYwEeEX2dqaTkr%2BxSxFQ2kwDgU0cxsf2cIRb1iKMwLpYbo9jW9TBBbaYyAQ%2Bw5mBSBENbZY%2B3T2YQHIYYA40Id2ayMYOcZR%2B0I5qYDPihpRiP2eNJESwqn2WEjjEDRj9s9QecEB7ZU4yBWgFFQCslGhWJ7keYdUX2YgvSda4XwYK8Hk3RDR0NL5SzwN4vAYLBs0aWI6EIW4zzAgB3J1MMrPDc32DJxyzMN4L9YgUIZQMLiuMMIwSbtaLjABJLcmAdP0IKgujiQIB5MNfUjMA/XCAQE4MhPDF43mPDIBBA7FkSQgRpJ3PcD2hHSQ2ufTI0JBBlJSBQQD2YA8DGHlvLxLxYnlFgdm3BRdwYHYAE4NCi/xJAANlogAVBAfP2ABxHy11heFBASZYXLcjyvOyvzCEpIL%2BR2YVRXFTAdnQQImEkJgAA50ASph/ASwIuCOI4eq4ZAuDapgmD%2BKh0DasxMASqgUrS6EsrGGEJPU1RCq3YrPJ2bzfP8yrgpqkV0nqnYZo0Dp0H8ZANEwLgHq4JgqHu2IotiDRbsCI4fskOKksxJVjgiZBvCwQ4jjcMZ8EEHkEBMI5sH2bEvARNqAH0CGBswjlB8HHkR6GCD%2BTAqHhxHkexDIAC9MCxnG8YYMGvAhomYdiVBPAppGUYOTnPGB3sDmUY0aFoWhTiDByngM64IEBCXUDonMvgUDk8BBCJgEOBKNHsq4LBE4gMbRwRMfWwgIHRPX0SB3Gv3mANpcN43TfRjGxmILwHAxqhLloDHWzJuhaAgI2XiEPA6bNCOqGUAhiBZEx/DcYHAgsMCs%2BznPc7z/OC8LuMU7TzE4z5Gd52o4ujgAEVlqgo5jouW9bsCS6Vcu0BItcs8R%2BvOjbofh8OVPO/2HknLedu64bhOyBHxec47suJ6n4hsFIyH67j%2BfDksBum9NJei5XwFy6dCbl9n3fE7NE%2BT7P8vGjoa%2Bd4jPeH8fsegcCWvGcdpEZ2gkmhxwSO7c2ntE4%2BwIEHMWodw4RhZAbUBbszYEExl7GBfsA5wJDhLKAtNMAikQS8ZYOwiEkLjinCwGgU612WKsfeCVqH%2BFofQu2gIlQEFuAWL8bFoEOGAbpVBcsIEYIZr/Kwq8iFYzAqIFIVceGI0znzHYRNtRMFUFaG0DAhjXBFFxB40I6x4gQsbcesje4BnWIIFR2c9jqKhvsMoFRyxwWMVeTCZjhJy3HugzGa4ABU697FgUcRo2uXp/RgOIP4j2wT16b3QPYiJziomNAbm8VwN5iDLnllwLC7owyYFuAIAE69OFxgCXIoJl8UlHFUVnNJbh%2Bx5SfEQLczp4mQOCS/WgYTml7A0QOfK6EoL9PTvXFBxBYniMCSooWXDexBNWWs9ZGzNlbO2Ts3Zez9lBM7kE/YlsCA7AkgoU5Ow1RKGxpiA5DzHlPOees9RQNMRjC9OCLxKYEJc0ljMuZNTTkQEBWghJ%2BwgnIFiGaKxAZK6Bh4WaGpiSP6J04SYDO488C%2BigNC7e/cXFyFCKEZOgQ07ktZGi64/dCVRGJaSqpYFjQEA2JhKgYglCpOGc494sp6ApNXtiU58ZBF3PPgGWIABaSmCilEMkJXKxFNFGnj2hTKpG3dBDb32HQ1Vq91WU3Xjqpy89FkGulUa42ySTXUv3pnJV1FzUSsNUjeptqXhmv1S6y1SN%2BkevjonZ1cYWVsufN7TA3KnGtKED7LYCgFD%2BwGb/f%2B7zASfMTD8p2AsAUu1EZGeZGMLkgrBWIlFkLoUYqxavUNxBMKuuwI6nhzDzAJUlRq7AoTvWYtTcs9NU5vnJmzf84RDkgUexuZgAgoK82zPBb0itsQq3SJ9R2rVa5CV6pXXGBt7rFW%2Bs7RY71O6D3%2Bv3R2rt27kJTrDdAyNjS1EaNjcgeNibLhTKWTGF536f2/sOWXY5ZwQGOW4sazeABHLwmAoM7FrpgSD0HHgQFSulOiodzQYBxQyHxlSlR/vwwR3Zby%2B0ZsHQwX5YYR2loLTUiDUGoMAHljyKUIJgGdwHx0LqhTCmxUI1zlqXenK9OKYQNvXZDbA57ZUIuokyzFV64zYneAHHkJzmbGknF0ia48T0dr3ZYQlXBg3yIPXu2eoLTPOgJbPBtSTSLJyOO8dtVq5b7EJLu7px7Dh/3HjULlq95M6b5i41Aa5lMS1U3IFI6AvSPA8D3BIQWxO2I3TZg94nrD7CM15nt49sQAHUlKRtXkEizHb%2BkOfrrEYzwr1PukYGuSZFryv8vtYZ4zDaz3mc6612laWL3Wvs5DJztnjZuec36/lxna3eIjVGp9cbMAJqTR%2BtNpGkzkeHZ4Udrsy0ezo4h9jIi517a49Cs0aA%2BMVghYJqReWDjhYGY57FuLks9wk1JzVMmeFMpDTeutD4IL3szi0j4/LRz3f2IV1j49Ss9boJVsMNWDjrWQBphrz4pvNcpmegzs9stXvh5LQlZWce9brp9w9ctkkOZGwe417nT1Ta8297VlOMsHwJ%2BPGb4aoPzecc%2B19K2U1LPW1moBOadv5vATUuDCGoNHbHfOiRwTzvXa47drh1aJXYh%2BKuFV7wXuifSyl6zA9fvXtZQDjlQP%2BetL5XQCHQrVTdJK7EHV3G9OeavbVtH9XtX1LVZZq%2B1h2ss%2BDwCEnHmQ8U/6y5yMNPhsTap5Gcb0eGmE5N%2B99npupVZem/92bfOH2g8F0tt9yate9qWTsQjdf6//vPoB2dRjzGueUCaDkMJ8voaiKF7UmGqAMliZwhvY%2B/3EaVGLodEuqOzs4xIjvmAOSK92zRiF3HkUb545d1acKRRUFucuyHevlHPdXiJ0nmrTd9fN0FnnNuuUl55fb8HgqJWX9Z6l1pB%2Bj/38L4Dk/iDi/mDo7u/jugIHvtHPTGuPUutO%2BDqunvsHnqNn4l5iEogfThYmwhAHARtPaj6IflOsnGYP4Mnk2jRP4P/F5jznenbvGItstu%2BiLmml%2BuPuwc8kclLMBq3r4qnriFRM2v7MzJCNCBAD3hLCFmuFqIPsPsbKPhwYoQcpPh8gOhthRnCtRjLh7G4N9rcKvtLibOWtxsfjWgAWJnoc6rlmtmoeLo8JofPsrpjPFoIAYSduvmdproFmYVbvWlnnYt2j5jYV8uoVtrmhxk4UWgoI9m4QvoEouqYRKjzlfo2jfkjOznoeiIEdXqoSEXYZRttloUYR7BcqfvoUUYWqrl4drn9r4cbmumkdQXftkZ%2BvbHjL6IUtiKEI%2BKcAIDQMQGwACPliQBrH%2BAxphEyFwFKhoIENMYEDcMPuyJhBSFSBhnVJKNKPysFGwUobsdslwVqJaPsHIAwNeMlEtmuAxr6EBsdlknEvcnsY8RspPrjPjKzITFDDDE4DzJJm0YAo8BYHIO8O8BjMlNgEIMlBjEIBJAAFrIySA14HCHGnHWibjppKDNDHGnE5SuQEDuS7QdgoTHQPALgRD8A7DaAEBQgUlUkaBXQYoOwhxALWgYwogYnTpsAJrVjHzGIEAsjoBQTyZXi4pgCQC8mMKW5hqhASRRDYAynvAMb7xmDjYQAaQFgOAgDmDKnHC8lKkkHKmclVg1j2I9r7AMSO4wgaAMlHD/HmheAYzewMBYwXEQDimfiCkZwBjbiQqGnck6q8nWz2KX6%2Bk1gsg84hnA7eb1zmn0CWkYrCz7ADBqb8AwhqhUCEgYwYzSmymZnPhQRezazbR4m7RYD8gpCO7HTIAgg7AcrgY1mO6smkQYy/IYwJBtgmwFlGDwwECKh/FMmPDZlylRAKlQnJQqgQAAAaJBRwE5AC/ZiZMpQ5I5o545U5C5sp8pDGK5k51ptpg5m5%2BwmZ7wEkoQ2AuZWp42F5%2B5w5W525mZg5mZ8ZXCrxvoGgwWcGdoUh7AxxZYeI6UnSWA4YOsPil2Kka4hiIFLeoSmIdo8I2kjhYiEQ%2BYnY06FRxhauC4oYxyLACgwAiRcYrYCIVAEAWpJcDA5g/g0IhIWpZoOFeFxmrEEACIV4Oqb5QZkMrSFhiiyqkMmceAoelgFuhFggxFF5FFKSZgZgF2WBfibC/FVBWRV6wlBAolklZFWpilq2z5RwrgOKSymF1wxy0KEYzpYweC4sYcsFeAaIQmPSKukq2BFggJwJoJ4JkJ0JcJ9COq8mdCPmLOCFHh9lwcFlo4cciBAV2hkCWCvswVCCxlcsmlBqEVxRC6loBAYVhKPluWXmLJbJCQ06pFklZoopaFJRly2JJFZgCUaVccjCHWyVhaIKraNVEYscQJIJYJEJUJsJ2A0l8hxmuVCa%2BVlVZgNFUu7hkVEixaFVzV2JtViVEqg17JI1Y1pVkCuh3F1ElVCUsVEsoVSCZuNw7VrlXVHl2AC1cYS1w1hVUl41cRGMLhBVVVu1AqtVh1VpA19peVzQK1RV%2BwJVDVwK0RAc21L1%2B1ZCF1dprJQ1P1N1F2gNZVZRoN8Ce16A81n10Ny1cNLYKNr1EYFcm1zah1LVLwBN8qcYkNV1sNalf1YNaN%2BN4mfcse9cJN5MjNYElNX1MNT1o1tNuN4N5MxqM8TRXihAcck8Y2HNGN31PNY1dN4tdmkezNot6V%2BNit%2BwnNmN11NNt18t%2BN7q2ct%2BKt4tBtWcmtMtv1ut/N9NpN/qhtytrNPIdtZt9VERp2Eik6T11Vc1SC0t3NltZoetpN7NNcItjtIdGtftWNOtgd1tJtVm9tYdPtpNptUtOVXN0dvNVt%2BCeNttrWidhKjtztadV6POW6WlSoBlkKDlLwplsCNYkQKEllcF%2BF6u9l8VkYNCzlHVbl3VnlVB3lGcvl1BhOCNC6QdvoiqY9Ei0VsCL1Fm/VXmjFzFeArFvFLFGi3dJ17lPV69/FBmgl483hEql1Gd2tWdxVYAa1U1wNBCraE9dVXmp9Wt1NF9d1kRB2Cu99cdrVV4j9CmUNFt2NANbtgVFsCgSN39OdAt/9QWVNstf119zhKWyN0DNtVAlWJO%2B9FgXAsDAWQRi1Z9r9Y1IDx291n9bGUDIV6DZoGgqgA0eDhDL9CDt1SDURMRVDocMD5t/twDV9094DkDz1P9ENDFXwTF2qq9m6e9HFR1LlnVO9cJe9AlFgcmNRYE5aSt9cUqXOq8z9QDMd797tmMcu9GlDwjaDccZoraaIkN%2BjvDhjWjhdkN1hSRAB5dLBfaVdRlTkddGM9oCQLGDwEAVlNld2q85aHdiWbCW9CjfdkmA9mVQ92Vo9oDk18RE94VaTKVM9Yq5lcVuGS94jK9a97Fm9x1cTZ1yjB9qjR96jO6AjGMFDqD1DVjf9xmrjcYy9kjpTD6UjzisTvdVTQZKjajADbDFDTGgTRWLTXDNDV41guDHTBDXTxTPT0jZTAzFTQzu9IzNTYzQWmjOqOjxm9jmdq1jTpjh2nDqNbTNjjDWc8DAdn4xNBBSzOWKzkpAOHjVe%2Bl3pPjJlvJQcneITLdtlETEKUTcSbCRwXlST7CflqTZDkRa4mTU92Thas9%2BTBCULkN3Ta4/T9chSmzrSZBezVgh9%2BD4zjTkzzGMzNzudVAZoeALjnz%2BLG9s8bFfTsjJw5L1gBzejbd8RWjyBujJ9gDDjb9bDS%2BK%2BDLAt1jVV6AzLDzYETz2NTjs82DZgrLvatRYaPzrjU%2Bth3jckgcvJCggZ4TTDjpddC9tdQLL1mtNrAZHdfjDdCQYgTrJEtrrrQLATxAQTmAXrTpLrvjDrnekNZdVhBDSozFMoEQoL1lrd3jxoCglwqW9cuoddFrkNylqlvNlF6lf1EAqb6btO16abtAa41FklgIJK%2BwCjQg/YgIQgQg2A/8qc5FklkbABpbVb/1YAm60b1euMulVAHAqwtAnA/gvAfgHAWgpAqAnAacNTbE6wmwhMuMPApABAmgE7qwHIAQnQU7HAkgs7e7i7nAvAHknQu787E7pAcAsASAZZjuZAFAoKCo5ZWYIAwAUgUl4siIHkEAsQF7lEzAxAtwnA274HTQtwDGsQHYGp3AvA/Ik44xtAUH97pAgFXgwAuhEsHkKHOH7ohgwA4g2H%2BAxoDgeADoRHC7GknYuI2w27g4J7C7YIbIcHHgWAF7icVo0HvAx4nMSgcGMoRgKYoA97qwVABgwACgAAakRHRAxgWHO9u/wIICIGIOwFIDIIIIoCoOoNh7oFwPoGRygCo/oHgLEB5JAKsKgKuFkER7wP68SFgHZ9bF0ChVkC4KRJMO0GZ2EPMK4sMFwPFKQGkBkJCAFyAGYGZ1F0UAwAMKF4kOF9IHYD5/ULMLF/F95zR9l30Cl/ohIBF58hMJ4G0HF2Z%2BV3MKUCV%2Bl6sJBBsFsBIJO9O%2Be9h0uxwPsKoG1AlFKglJIPsMAC%2BllpIDyMqRALgIQF8McLg7wHe1oIwqQIe/4Me5wGe6QHOwu919eyALe3uytye2YJ17t1ezu0d6sEZL55IEAA%3D) vs [Index Approach](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGISdKuADJ4DJgAcj4ARpjEEhoAzKQADqgKhE4MHt6%2B/tKp6Y4CIWGRLDFxXIm2mPZFDEIETMQE2T5%2BATV1mY3NBCUR0bHxSQpNLW25nWN9A2UVIwCUtqhexMjsHAD0WwDU5gm7bnjrXgbEuxYAngSYl15UVLG7AGIku9jlmOhY6LsAgslkrQ8KJ6gpdhAAJIMLCqS5MJToRYmDT/HYArwEBAkECXYh4Qy7ADSCC8XkhACkzlddmYNPTFpCogTDABrMleAB0aBYTIgLFQ/AUVzCAAFgCwmHQeag%2Baj0XsAEqYRHOXYAWTEtFQyF2VGImEwpF2LFCeBYPl2qAAbrEEKq/hbgZg2IImPVrVRdkxdsgTsgzs1dlEHk9iFyFRjdrteo49aEQWE/ghDOh6MQIfwLgpkka/mndqqlF7dl4FExgHdQrsbc0nOXdsliKhtJgHApI2jo7sYXCQ4jvrtnfQ3U1Pfg7cQq1mWyxdqnMwg2QwwBwIVP0gITekGBthwRhwoo3sY8W8M9s6a7kRdsAW2yubsABKoADumCnpqYtN5dw9fppIeQq7Lm%2BY%2BrC1rJI4ZoVvUXaKjGAK0AoqBlkoEJRDcdxRIOfzEB6DoXNiRKEK6A5Ivq7zns8XiwrEMxRHQhBXCeSFvg6DDoUOV7pCwLoXFhtw%2BsQBG/gGQaCWGDEIT2UILkwdo%2BihaFvgSBC3FxRKpKEtyCXhfoSecIbSRcm6ZE%2BbEWAZAgKUuK5rhC/qnMZoaPMMC4ackCggDswB4GMXL%2BdiXhRLKLBbIuCjLgwWwAJwaHFACskgAGxsQAKggAW7AA4gFh4QMoqC6bEix4ggXk%2BX5BVBYQZJhbyWyCsKoqYFs6AAOxMJITAABzoKlTBJalnVcAkCQjVwyBcH1TBMD8VDoH1ZiYKlVCZdlEL5WMkJ9pgqjlZ5BDeb5Wz%2BYFwUNeFzVCmkbVbCtDIaOgSXIBomBcJ9XBMFQH1RHFUQaG9nUJKDkjJelaIKgcoTIN4WD7AkbhjPgghcggJgJNguwYl4ul9QA%2BgQMNmAkcMI3cWMowQPyYFQGNYzjGLpAAXpgxOk%2BTe6U0jNPoFEqCeIz2O43sgueDD3Z7Moho0LQtBHEZwYWKZkL/Ar75Zu8ChsngQKhMA%2BypRozmBucqvucQhP44IRMwoQEAoibKLQ2TWA0GEhkuc0lvhjbBOE2MxBeA4hNUGctCE829N0LQEB%2B0IeDsyafvKAQxBMiYSVuDDnUWEhhdF8XJel2X5cV4X2e52iMY8kwyRMP6BC0kjAAi9yPEnKeV73fdF9XCp12gdGHlXCQdxoJr9zPs8xoPte7FybnhuPHdpxn09z9vxcL/8dcOgtu8T7sU87%2Bf5d73XTR0Mfk8Xw/Jd7yYnVt1zHuhHcZuSX7sQB3bQcM6hwINHOWccE5hiZN/C2pl/4ECJsHYB4dI6gNjgrKAbNMBCggY8RYWxMHYL9tnCwGhs5t0WMsfYZhUpEKSiQshrt/gKhbnmD2oEgEOG9ubX2sDbbwM5tDfOQ9QLJw5oeUQjdm6sQSAXHYWxqaaiYKoC0VoGBDAuCBMiLAIQ1mxDhUywjMHE0AqPLGBckJyIUfMZ4mjbjaOHFxPRJkrbCL4UTQ8AAqFesQzFi3kcjXYbcPS%2Bl/sQQxojjGH3QL4wuliAnPkdL2eiqhwns2MTfWgMSLE7AURlaUit9opMER3aBPCrZwPcWYqWTDuweLqfUhpjSmnNJaa0tp7SOkeKHh4pJhAthQgUA7AgWwVRKBJmiTpkypnTJmQ0%2BR0M0QzHjA4pMOEhaK1KcQUJFTCZDIgJs7ZbjjEeOQFEHcETxENybixE0RzPEr3TpnPOVhF4HN4YHQ8rpoJXDcAXLGHcX70NflUxeeBvRQFOe3f5uxwhyCCEELOnVc5IuZGGR5UKT6wvhRQ4RMZDQEDWFxKgYglC%2BLiW4V4%2BTvjCIxEM2MHDxn712CcqI7ciz8Rbr8kFTLTkAFomYSOuS3NlgqpHcpjHypm3iLjQoeRncVfooj8uxiPQQbLSEyOEZK7GUT1UKu1dgDJerNWL3xYS3YQDMBkpyQEoQocNgKAUBHTJxTqlLNBCsz%2BIZ1lcJ/u8gBAy9lvPKXc5lpzGGAuEWa4gXEDWipYlQ9K1DFXKuwNK7lL834LP%2BO6hMDBVnes8L6mBIbA6jMwAQfZystn%2Bv4Z48NzytVKoFasNV0KNUvJ5c2nViT236u7Ya/JxrO14oreay11r/EUrtcgB1Tqzh5yzTU5dWxZlrvXRurptcenHB9oJbCzjV7YAYAARy8Jgc9Ww26YDPReu4EAso5TfHHU0GAwW0icdKxhm6f2/rafM5dubPVewlhs6thzA7HtveegA8lONSZEq17ogwA%2BtZzAIMF2qGqIEahGgvBXG1th4mbQrjVcqRjD554aZTGDELxI5PhhMgQ0Y4FyOlxSmpmuroX7IHdxywuwuBZzMElTjKryMsQVZm4RtRSWL0jYvWjexwioEPPRhWT45DJHQB6O4HhR4%2BMU2J7AqriMn0I6PKhBcuBSeBYvDEyhywIFrGIc9TbU3proQajJZC2VRAVbSvcLHGCHh868gdRqePeaHdYQTwnRNkckZJk1TLo2OJDlamRfiFEzrnc6xdbrxwesTF60Dxayn%2ByOVB8956kPcJraW1DYb0NoEw4ebDuGR1i1eJHYRYLIQWbbdjUjA740t0o0hNL%2BoSWZdkTailLwqXRPszLJzLnvBWvCx5gxXmIv5N87KgLewmPBbVWFrtqbIsn145dmLAmhNUIS6NiTLd%2B2ptMyK0bRGrOCYVVNidWXyWxntZgR1%2BXXXZqAyVkDPrg2VcDte6DmA6t%2Bsa3W5rtyPnMpw42lbHwOWsXwwNr7ln/ntom6OglMbpsoVm9lgJi26DUrx8VZI623OLy8Z97bLjdupqiWQo7STmOuhC2xha7muO9uuwa/j1n4vGbG9Irrg2zMlJJ2q3lv2UuU/HRlydOWQdg4XRD5dUY/0W8t1u/eO7q0hgPdK3YygjRskhAAdRfSpw8Go31UFpKE79Vug8boAwqKH%2BbSuw/A7WomzvMBshRyW%2BHTWWWY5Ty1gQu0jHAUeGMzrNK9ifG%2BVjF4fWCMa7V%2BTjjU3iW04NwzpbZfifve%2BwooUVA8/V7HdT2vpLAfzcpUz5bTKucjZ5%2BGYhN2pdH1i%2B3vPj3FcvdYklN%2BOvdj/f1/3qdwPZ2g/nS6phdmze1OD6fmZ3SlZ7vt8JR3WJmLCojnucE7vPeqc1L7/3plA9n5/x00Piyis80C1s9ysGtk9%2BE3Al9E8Ks/5Q0WV89TVu9Y1nsktXsUtpNIdADgM7gQC4dYDA59NBBoCwD8CU8G1BEusptVcM0j8w8sDoc1ki08DrYjkBl1N45mCdk0MEDUskDm8W1SdhtzMUChUrgUR0DaCACPRisI8YcmDo80d7YFAi8W5iCUN0d4DcdeCqcuIp8VVW8J4q8JCl1qlYZvQuBusghERDwPAGAaBiA2A/g3cSA9YjBdgYMuJ6QzAuBeUNBOpfDOpLh/dWQuJSRyRX1WpxRJR8lwpzdf94jWkL8NRzRdg5AGBCBdgMpQdDwYNvRd16tO4J8JkEiSjGlQ8yYKYvBEZqZUYnARZsB35Y4vYLA5AXgXhCYMpsAhAMpCYhAoQAAtHGSQapDEZI9Iy0ecHNJQFoVI9IwqSqE6aqLYNsAiG6W4ZAbKOw1AZYggTDHYhgDQBkCNd2Jou4S0QmREaYytNgR1SsY0C1bIpkdANCQFYccFMASAW4MYChdfPgoIKEcIbAAEl4GDKhMwXYPECAA6PMBwEAcwcEg4L44jMwMwYTcEm4isKsXxTNXYDiJnSEDQY4hID%2BL2c4kOBgYmbIiAJEp4l4/OP0RcZlDEu4tlJEp2XxfrAUPfO4pkKbZkrErLHEvE%2BgAkiNaWXYAYJJfgSEFUKgPEQmQmf4wEhUi1NCYOQ2Y6ZIM6LAXkZIJnG6ZAIELYYlE9Y0pnC42EQmVZQmWIFsa2dUowDGAgeUN2Yk04iUgEoE8IEE3ojKJUCAAADWEwSADMaM9juCVK9J9N9P9KDI9MBOBJgxjMDKJJJIjM9MTN2AVJeChCCGwBVPhIhLBPjKjKTOTIVMjIVLFKYQqPMO62vRtBhVU3YFSJLGxBylvCwFDCNicVa00hzwtQdFAMPUM3%2BBtBKj%2BE4NCDAgcDUJj2OVORNA2ODB6W0WAB4JjGbF0ioAgHhOrgYHMCSghDxHhJNDXPEK6yvAgF0mHHVSRgLjwD5kX1QNpA5OsGsApybAJEEB3MLMPOiRRKXIHU8wsDwAYQVS3J/N3JRP3PhIvIK1dNcDBWqWXIuB6VOTDEpLGFQXlnjnHLwGRC0JjFDQwt5wsBaLaI6K6J6L6MGIOxPkBVIWBTX04NDRjlwu%2BD9hFQUPAIQQZRwvAVIvDHgteR4tIPR3NAIC4uhUYukzX3OMuNiErT3MAt2A%2BNYsDkDTmOgtSkkr9hxRYrEpYM0u0vMF0rmL9lTlaPaM6O6N6IGOwCAq/wVQUsdSUugrMFPNAPUKUL2TMr0sgREqZVcquI8q8o0oAUgJfJ0vYrjk4sgQxXXmsqorstouwCCpjBCvcpUrMCXKMp2UIOUuoVioVnitwUStPgytNC8AuLcpaDCtUvUvytYIUHYJirAVKvQH0qqqyvqpyryuQ3nIGRUKuHarQXoC6sCpcpqsUr6pgtUpKomr9nrhfIqoCseBWtEN2B6pmrqqKs8oWo6qWrDFlEsyQjJxPnWoZg%2ByQh2tqtCv6q/PGrKoZkd3nkMMuospOreu2umvuuyvmtyqeo4smo2t1TXmhSuoxkSULjutmv2q8sWpeq5CNQhs%2BsIGWtRtuv1WarLVBwrR0qurhr2oaqBqRtBuuu%2B3Oo%2Bo7ihpupjGJoesBpNHJuWvBuptX1pq%2BrBphuxvkt2qZoOrJqOuRqxves5ocQxpOrFt%2BrXymw7QQuXVQuZUVUwqRMJirDCAIjwonI3LLCxyEp8ToQopsuovsropXzZVkuYpV1xqa1ZrDG4sGsUMARDjDkWt42crXyvJvMfPbXvNvIURNpSpoocoDrAssA/OEQUxo2qv%2BrmqFpNCaudt4t2VasjjGpBu6oVUyoFoBsTu8vnOqzvUzrioppNDwAMq61zvjoRsarAAiv4WGoJ1Ls6uzrXxrvhtJoGvqx8sJkKtbuOvKouo7mvOsCEyqowOCrzoTq8uTt7qLtPRq2RzModseBNA0FUAmiruEV6rrqBsbqULatXpFopp3sXj3u7rUobrtqbuUJbpPuerPqqp9rVT9pPg0HDqfODtstDsGPDvfMsAmxjsLlDT%2BGhV5Rsw7rjq7sesPsJkR2XsHpepNDMuRCqs7pJsevAZpuHEnqP112pwVtNwVGVvQpXiwpAVtFiAQ1uAgHwsIooNcQNpAp/rNrSvooBXziYtX1tpTvEvcWBrLukvM1vr4rdpAQ9sNqeW9veGvLfrvI5O/uSt/vNpxjfMjqAejuo0LngeLtq0fqzrDArvwaXRjFfsPHfvviUaDpUfYbDo0asC0fkx0aQj0aXrvTgxofUhXuKtPssuHHHtMeEQscDo/q/tscotUbSoAc0YsGAdceIqxxwY7kgZzpgaweZsLpdsQZLsMeEeMcTXQfScvuwbWp%2ByE1szMd%2BJ0NPhoJMLRDIdVseEoejhd3od1qIv1qa2keIQSE4f2G4bkr4YXpdsPDXu9FIzEddqQSka/QVVCascEwiYCVE0cajpceruydTv0cwC8eIFod8dSgmZMaqZCbkd9sUay3fopUOHWecaZRAaQjAbZTSegdKayfgbjwT3ybbsKbQZMYwYycFq8pSchusFRLOcQNqeIcPwaZzSwKae1EoYUHZKYYvpqvJMoc9pafVsWruqxbZKEtac1tiDEAJbomxeJfVuoYOZ8YpYpKJYobxZdyqvlvqeqRvKlFCA6YIr1rIcNAUDODV2UijiRNRaqsgoIF/PmqPNgtUogEFeFazgSBeF%2BKFdoEPBPJRP%2BHhUyOoqECd3%2BCECEGwDfhzgPJRLZb4KVc1evr7WMK5iQqoA4GWFoE4CSl4D8A4C0FIFQE4FzjidAlWHWCpjJh4FIAIE0FdeWDZBACSinndY4EkC9ejb9c4F4B8inijZ9dddIDgFgCQF1KZzIAoH2TlD1IzBAGACkFyvlj0h8ggCiDTaYmYGICuE4AjdbeaCuBgyiDbFhO4F4F5DHA8NoA7dzdIC7K8GAEgIVh8iHanddEMGAHEEnfwENAcDwDtAXd9YOnbCxE2AjdKiTd9ZBBZB7Y8CwDTYzgtE7d4CnEFiUGvSlCMCTFAFzeWCoAMGAAUAADULw3wYM8xvWI3%2BBBARAxB2ApAZBBBFAVB1BJ3dAuB9AV2UBAGbBz2fJIBlhUBoJMgF3eBaWCQsBsOnYuh2x6gXBYRJg/AYPghP5rFKgUoUg0gMgBBaOJB8g2P6g5h1EJAWO7BKOehxhWhPB2guOKOt2BBegWg%2BOvhmPpAZgJhxPcgYPlP%2BhGP%2BOuAUplhUI1gNgJA3WPXU3J3/WOBdhVA%2BpUpeVUpJA7xZ1BNJAuRwSIBcBCB3gDghNeAc2tAKFSA42E39BOAU3SBvXfXzPM2QBs3o3/Ok2zBTOIuM3I3YvlhzJnBJAgA)
    - This might be due to the need to copy over the pointer address value to the main register

* This circular buffer uses a bunch of strategies to be more thread safe
    - Use of volatile keyword to avoid compilation optimization of the head and tail pointers
    - Reader and Writer function usage of the head and tail volatile values are atomic and separated.

* Functions are static inlined so that it's easier to include in various project as a header file

* Functions are written in such a way that each function can be deleted if developer seeks to simplify the code for easier review by removing unused code for their project

* Malloc free. You can easily add it in externally if you really need to anyway via your own a wrapper function. By excluding it, it would be easier to integrate in microcontroller projects.

* Some circular buffer implementation has fixed buffer predefined. Opted against it to ensure this code can be used for multiple different buffer to reduce code size. Also it's not too hard for users to directly edit with capacity value hardcoded.



## Circular Buffer In Memory Representation

```
  0                                      X Capacity
 [ ][ ][ ][ ][ ][D][C][B][A][ ][ ][ ][ ][ ]
                 |--->>---|
                HEAD     TAIL
               INPUT     OUTPUT
```


## This Implementation Lock Free Approach (No Mutex Thread Safety)

The issue with wrapping the index by X for empty/full detection (e.g. [god233012yamil/Circular-Buffer](https://github.com/god233012yamil/Circular-Buffer) implementation uses this approach) is that to maintain lock free thread safety you would need to leave one slot free to disambiguate between the full and empty state.

We can solve this issue by projecting the index beyond X so we have a virtual index beyond our memory capacity.
The actual reading/writing of memory slots is still between 0 to X, however our buffer size count will be projected across a virtual buffer twice the actual capacity.

How this helps with determining full/empty state is that the TAIL index and the HEAD index will never overlap

```
  0                             X :X+0                           2*X
 [B][A][ ][ ][ ][ ][ ][ ][ ][ ][ ]:[ ][ ][ ][ ][ ][ ][ ][ ][ ][D][C]
 >---|                            :                            |--->
     TAIL                         :MIRRORED                    HEAD
     OUTPUT                       :INDEX                       INPUT
```

## Other Circular Buffer Implementations To Consider

* [god233012yamil/Circular-Buffer](https://github.com/god233012yamil/Circular-Buffer)
    - Appreciated god233012yamil's [Technical Writeup](https://github.com/god233012yamil/Circular-Buffer/blob/main/technical-docs.md)
      since I learned more about consideration needed to add lock-free thread safety.
    - Ultimately opted against this approach as it relies on a slot that must always remain empty 
      to ensure clear disambiguation between empty and full buffer state (one slot empty).


## Recommended Readings

* <https://www.codeproject.com/articles/43510/lock-free-single-producer-single-consumer-circular>
* <https://andrea.lattuada.me/blog/2019/the-design-and-implementation-of-a-lock-free-ring-buffer-with-contiguous-reservations.html>


## History:

Originally from:

 - This Gist (Pointer): https://gist.github.com/mofosyne/d7a4a8d6a567133561c18aaddfd82e6f
 - This Gist (Index): https://gist.github.com/mofosyne/82020d5c0e1e11af0eb9b05c73734956
